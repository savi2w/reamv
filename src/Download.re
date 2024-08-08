open Config;
open Exec;

[@bs.module "path"] [@bs.variadic]
external join: array(string) => string = "join";

let from_crunchyroll = (url, folder) => {
  let file_path = [|folder, "input"|] |> join;
  let command =
    "youtube-dl --format "
    ++ video_quality
    ++ " --recode-video mp4 -o \""
    ++ file_path
    ++ ".%(ext)s\" \""
    ++ url
    ++ "\"";

  let%Async _ = exec_async(. command);
  file_path ++ ".mp4" |> Js.Promise.resolve;
};

let from_youtube = (url, folder) => {
  let file_path = [|folder, "music"|] |> join;
  let command =
    "youtube-dl --format "
    ++ video_quality
    ++ " --youtube-skip-dash-manifest --extract-audio --audio-format mp3 --audio-quality "
    ++ string_of_int(audio_quality)
    ++ " -o \""
    ++ file_path
    ++ ".%(ext)s\" \""
    ++ url
    ++ "\"";

  let%Async _ = exec_async(. command);
  [|file_path ++ ".mp3"|] |> Js.Promise.resolve;
};
