open Config;
open Crunchyroll;
open Download;
open FFmpeg;
open Music;
open Util;

let prepare_episode = folder => {
  let popular =
    "/videos/anime/popular/ajax_page?pg="
    ++ string_of_int(get_random(0, crunchyroll.popular));

  let%Async anime = popular |> get_anime;
  let%Async episode = anime |> get_episode;
  let%Async file_path = folder |> from_crunchyroll(episode);

  folder |> split_file(file_path);
};

let prepare_music = folder => {
  folder |> from_youtube(get_music());
};

[@bs.module "path"] [@bs.variadic]
external join: array(string) => string = "join";

[@bs.module] external temp_dir: string = "temp-dir";

[@bs.module "uuid"] external v4: unit => string = "v4";
let get_uuid = v4;

let bootstrap = () => {
  open Js.Promise;

  let folder = [|temp_dir, get_uuid()|] |> join;
  let%Async artifacts =
    [|folder |> prepare_episode, folder |> prepare_music|] |> all;

  let%Async result = concat_files(artifacts[0], artifacts[1][0], folder);
  result |> resolve;
};
