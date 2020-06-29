open Config;
open Crunchyroll;
open Download;
open FFmpeg;
open Music;
open Util;

let prepare_episode = folder => {
  let popular =
    "/videos/anime/popular/ajax_page?pg="
    ++ crunchyroll.popular->get_random(0)->string_of_int;

  let%Async anime = popular->get_anime;
  let%Async episode = anime->get_episode;
  let%Async file_path = episode->from_crunchyroll(folder);

  file_path->split_file(folder);
};

let prepare_music = folder => {
  ()->get_music->from_youtube(folder);
};

[@bs.module "path"] [@bs.variadic]
external join: array(string) => string = "join";

[@bs.module] external temp_dir: string = "temp-dir";

[@bs.module "uuid"] external v4: unit => string = "v4";
let get_uuid = v4;

let bootstrap = () => {
  let folder = [|temp_dir, ()->get_uuid|]->join;
  let%Async artifacts =
    Js.Promise.all([|folder->prepare_episode, folder->prepare_music|]);

  let%Async result = artifacts[0]->concat_files(artifacts[1][0], folder);
  "AMV created in: "->Js.log2(result);
  ()->Js.Promise.resolve;
};

bootstrap();
