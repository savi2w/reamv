type chunk_offset = {
  ending: int,
  opening: int,
};

type chunk_size = {
  maximum: int,
  minimum: int,
};

type chunk = {
  fragments: int,
  offset: chunk_offset,
  size: chunk_size,
};

type crunchyroll = {
  hostname: string,
  popular: int,
};

let audio_quality = 5;

type transition = {
  maximum: int,
  minimum: int,
};

let chunk = {
  fragments: 22,
  offset: {
    ending: 180,
    opening: 300,
  },
  size: {
    maximum: 6,
    minimum: 4,
  },
};

let crunchyroll = {hostname: "www.crunchyroll.com", popular: 1};

let transition = {maximum: 300, minimum: 200};

let video_quality = "worst";
