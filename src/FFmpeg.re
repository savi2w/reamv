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
  let output = [|folder, "output.mp4"|]->join;
  let names = [|"fade", "fadegrayscale", "crosszoom"|];

  let transitions =
    (episode->Array.length - 1)
    ->Array.init(_ =>
        {
          duration: transition.maximum->get_random(transition.minimum),
          name: names[(names->Array.length - 1)->get_random(0)],
        }
      );

  let%Async _ = {audio: music, output, transitions, videos: episode}->concat;
  output->Js.Promise.resolve;
};

let get_duration = file_path => {
  let command =
    "ffprobe -i \""
    ++ file_path
    ++ "\" -show_entries format=duration -loglevel quiet -print_format csv";

  let%Async result = exec_async(. command);
  let duration =
    '.'
    ->String.split_on_char(
        ','->String.split_on_char(result.stdout)->List.nth(1),
      )
    ->List.hd
    ->int_of_string;

  duration->Js.Promise.resolve;
};

let split_file = (file_path, folder) => {
  open Js.Promise;

  let get_fragment = (index, chunk) => {
    let fragment = [|folder, index->string_of_int ++ ".mp4"|]->join;
    let command =
      "ffmpeg -ss "
      ++ chunk.start->string_of_int
      ++ " -i \""
      ++ file_path
      ++ "\" -t "
      ++ chunk.duration->string_of_int
      ++ " -c copy \""
      ++ fragment
      ++ "\"";

    let%Async _ = exec_async(. command);
    fragment->resolve;
  };

  let%Async duration = file_path->get_duration;
  let%Async fragments =
    get_fragment->List.mapi(duration->get_chunk)->Array.of_list->all;

  fragments->resolve;
};
