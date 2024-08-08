open Cheerio;
open Config;
open Util;

type options = {
  headers: Js.Dict.t(string),
  hostname: string,
  method: string,
  minVersion: string,
  path: string,
  port: int,
};

type response;

[@bs.send]
external on:
  (
    response,
    [@bs.string] [
      | `data(Node.Buffer.t => unit)
      | [@bs.as "end"] `_end(unit => unit)
      | `error(exn => unit)
    ]
  ) =>
  response =
  "on";

type request = {
  [@bs.as "end"]
  _end: (. unit) => unit,
};

[@bs.send]
external _on: (request, [@bs.string] [ | `error(exn => unit)]) => request =
  "on";

[@bs.module "https"]
external request: (options, response => unit) => request = "request";

let get_source = path => {
  let hostname = crunchyroll.hostname;
  let headers = {
    open Js.Dict;

    let dict = empty();
    dict->set("Accept-Language", "en-US,en;q=0.9,pt-BR;q=0.8,pt;q=0.7");
    dict->set("Host", hostname);
    dict->set(
      "User-Agent",
      "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.116 Safari/537.36",
    );

    dict;
  };

  let options = {
    headers,
    hostname,
    method: "GET",
    minVersion: "TLSv1.3",
    path,
    port: 443,
  };

  Js.Promise.make((~resolve, ~reject) => {
    let callback = response => {
      let data = ref(List.init(0, _ => Node.Buffer.fromString("Xi <3")));

      let on_data = chunk => {
        let next = List.init(1, _ => chunk);
        data := List.append(data^, next);
        ();
      };

      let on_end = () => {
        resolve(. Node.Buffer.(Array.of_list(data^) |> concat |> toString));
      };

      let on_error = exn => reject(. exn);
      let _ =
        response
        ->on(`data(on_data))
        ->on(`_end(on_end))
        ->on(`error(on_error));
      ();
    };

    let on_error = exn => reject(. exn);
    let _ = request(options, callback)->_on(`error(on_error))._end(.);
    ();
  });
};

let get_anime = popular => {
  let%Async source = popular |> get_source;
  let anchors = source |> get_anchors;
  let path = anchors[get_random(0, Array.length(anchors) - 1)];

  path |> Js.Promise.resolve;
};

let get_episode = anime => {
  let%Async source = anime |> get_source;
  let anchors =
    List.filter(
      anchor => Js.String.includes("episode", anchor),
      source |> get_anchors |> Array.to_list,
    )
    |> Array.of_list;

  let path = anchors[get_random(0, Array.length(anchors) - 1)];
  let url = "https://" ++ crunchyroll.hostname ++ path;

  url |> Js.Promise.resolve;
};
