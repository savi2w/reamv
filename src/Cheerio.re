type attribs = {href: string};

type element = {attribs};

type cheerio = {toArray: (. unit) => array(element)};

[@bs.module "cheerio"]
external load: (. string) => (. string) => cheerio = "load";

let get_anchors = source => {
  let anchors = (load(. source))(. "a").toArray(.);
  Array.map(element => element.attribs.href, anchors);
};
