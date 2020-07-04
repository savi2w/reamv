open Util;

let musics = [|
  "https://www.youtube.com/watch?v=LSvOTw8UH6s",
  "https://www.youtube.com/watch?v=HdnTSXUWd3E",
  "https://www.youtube.com/watch?v=ytO2Ymslc9c",
  "https://www.youtube.com/watch?v=2r-JBk5b56U",
  "https://www.youtube.com/watch?v=y9x2CHQClTU",
  "https://www.youtube.com/watch?v=k6PAakX2k1U",
  "https://www.youtube.com/watch?v=mz_9N3Ggz38",
  "https://www.youtube.com/watch?v=eVTXPUF4Oz4",
  "https://www.youtube.com/watch?v=kXYiU_JCYtU",
|];

let get_music = () => musics[get_random(0, Array.length(musics) - 1)];
