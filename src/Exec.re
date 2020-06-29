type exec_callback = (string, (Js.Exn.t, string, string) => unit) => unit;

[@bs.module "child_process"] external exec: exec_callback = "exec";

type exec_result = {stdout: string};

[@bs.module "util"]
external promisify: exec_callback => (. string) => Js.Promise.t(exec_result) =
  "promisify";

let exec_async = exec->promisify;
