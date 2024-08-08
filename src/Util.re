let get_random = (minimum, maximum) => {
  let inclusive = 1;
  let random =
    Js_math.random() *. float_of_int(maximum - minimum + inclusive);

  (random |> Js_math.floor_int) + minimum;
};

type chunk = {
  duration: int,
  start: int,
};

let get_chunk = {
  open Config;

  let next_chunk = 1;
  duration => {
    let fg =
      (duration - chunk.offset.ending - chunk.offset.opening) / chunk.fragments;

    List.init(chunk.fragments, fragment =>
      {
        duration: get_random(chunk.size.minimum, chunk.size.maximum),
        start:
          get_random(
            chunk.offset.opening + fg * fragment,
            chunk.offset.opening + fg * (fragment + next_chunk),
          ),
      }
    );
  };
};
