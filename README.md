# ReAMV

ReAMV build a randomically generated AMV. That is all

## Prerequisites

- FFmpeg `>= 4.2.3`
- Python `>= 3.8.3`
- youtube-dl `>= 2020.06.16.1`

## Usage

### API

```javascript
import reamv from "reamv";

const amv = await reamv.bootstrap();
```

### CLI

```sh
$ npx reamv
```

## License

This project is distributed under the [MIT license](LICENSE)
