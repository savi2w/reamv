let get_random = (maximum, minimum) => {
  let inclusive = 1;

  Js_math.(
    (()->random *. (maximum - minimum + inclusive)->float_of_int)->floor_int
    + minimum
  );
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

    chunk.fragments
    ->List.init(fragment =>
        {
          duration: chunk.size.maximum->get_random(chunk.size.minimum),
          start:
            (chunk.offset.opening + fg * (fragment + next_chunk))
            ->get_random(chunk.offset.opening + fg * fragment),
        }
      );
  };
};
