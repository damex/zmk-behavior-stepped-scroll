# zmk-behavior-stepped-scroll

Emits one `INPUT_EV_REL` report per binding press, with axis (`INPUT_REL_WHEEL`
vertical, `INPUT_REL_HWHEEL` horizontal) chosen by DT and value by binding
param. Pair with `behavior-sensor-rotate` so each encoder detent fires one
scroll tick.

Fast spins scroll linearly with detent count, no runaway like velocity-based
engines (`input-two-axis`).

## Install

Add it to your `config/west.yml`:
```yaml
  remotes:
    - name: damex
      url-base: https://github.com/damex
  projects:
    - name: zmk-behavior-stepped-scroll
      remote: damex
      revision: v0.1.0
```
For a local checkout, build with
`-DZMK_EXTRA_MODULES=<path>/zmk-behavior-stepped-scroll` instead.

## Configure

DT example:
```dts
#include <zephyr/dt-bindings/input/input-event-codes.h>

/ {
    behaviors {
        stepped_scroll: stepped_scroll {
            compatible = "zmk,behavior-stepped-scroll";
            #binding-cells = <1>;
            input-code = <INPUT_REL_WHEEL>;
        };

        rot_enc: sensor_rotate {
            compatible = "zmk,behavior-sensor-rotate";
            #sensor-binding-cells = <0>;
            bindings = <&stepped_scroll (-1)>, <&stepped_scroll 1>;
        };
    };

    scroll_listener {
        compatible = "zmk,input-listener";
        device = <&stepped_scroll>;
    };
};
```
Then add `&rot_enc` to your keymap layer's `sensor-bindings`.

`param1` is the relative-axis value sent per press. Use `(-1)` and `1` to make
each detent equal one HID tick; larger magnitudes are accepted but uncommon.

`stepped_scroll` is itself the input device; the `input-listener` forwards its
reports to HID.

## Limitations

- Central / non-split only. ZMK runs sensors and HID on the central in a
  split, on the device itself otherwise; the build gates this behavior on
  `(NOT CONFIG_ZMK_SPLIT) OR CONFIG_ZMK_SPLIT_ROLE_CENTRAL`.
- Vertical scroll (`INPUT_REL_WHEEL`) and horizontal scroll (`INPUT_REL_HWHEEL`)
  only. Other relative axes don't have a useful interpretation for one report
  per press on a HID-composite device.

## License

This module is MIT.

Dependencies (each keeps its own license):

| Dependency | License |
|---|---|
| ZMK | MIT |
| Zephyr | Apache-2.0 |
