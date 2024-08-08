open Config;
open Exec;
open Util;

type transition = {
  duration: int,
  name: string,
};

type concat = {
  audio: string,
  output: string,
  transitions: array(transition),
  videos: array(string),
};

[@bs.module "path"] [@bs.variadic]
external join: array(string) => string = "join";

[@bs.module] external concat: concat => Js.Promise.t(unit) = "ffmpeg-concat";

let concat_files = (episode, music, folder) => {
  let output = [|folder, "output.mp4"|] |> join;
  let names = [|"fade", "fadegrayscale", "crosszoom"|];

  let transitions =
    Array.init(Array.length(episode) - 1, _ =>
      {
        duration: get_random(transition.minimum, transition.maximum),
        name: names[get_random(0, Array.length(names) - 1)],
      }
    );

  let%Async _ = concat({audio: music, output, transitions, videos: episode});
  output |> Js.Promise.resolve;
};

let get_duration = file_path => {
  let command =
    "ffprobe -i \""
    ++ file_path
    ++ "\" -show_entries format=duration -loglevel quiet -print_format csv";

  let%Async result = exec_async(. command);

  let float_duration = List.nth(String.split_on_char(',', result.stdout), 1);
  let int_duration =
    String.split_on_char('.', float_duration) |> List.hd |> int_of_string;

  int_duration |> Js.Promise.resolve;
};

let split_file = (file_path, folder) => {
  open Js.Promise;

  let get_fragment = (index, chunk) => {
    let fragment = [|folder, string_of_int(index) ++ ".mp4"|] |> join;
    let command =
      "ffmpeg -ss "
      ++ string_of_int(chunk.start)
      ++ " -i \""
      ++ file_path
      ++ "\" -t "
      ++ string_of_int(chunk.duration)
      ++ " -c copy \""
      ++ fragment
      ++ "\"";

    let%Async _ = exec_async(. command);
    fragment |> resolve;
  };

  let%Async duration = file_path |> get_duration;
  let%Async fragments =
    List.mapi(get_fragment, duration |> get_chunk) |> Array.of_list |> all;

  fragments |> resolve;
};
