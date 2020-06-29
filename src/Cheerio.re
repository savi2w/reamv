type attribs = {href: string};

type element = {attribs};

type cheerio = {toArray: (. unit) => array(element)};

[@bs.module "cheerio"]
external load: (. string) => (. string) => cheerio = "load";

let get_anchors = source => {
  (element => element.attribs.href)
  ->Array.map((load(. source))(. "a").toArray(.));
};
