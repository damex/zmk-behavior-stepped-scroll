// Copyright 2026 Roman Kuzmitskii (@damex)
// SPDX-License-Identifier: MIT

/*
 * One INPUT_EV_REL report per binding press. Pair with behavior-sensor-rotate
 * so each detent yields one HID tick; no velocity, no runaway.
 */
#define DT_DRV_COMPAT zmk_behavior_stepped_scroll

#include <zephyr/device.h>
#include <zephyr/input/input.h>
#include <zephyr/kernel.h>

#include <drivers/behavior.h>
#include <zmk/behavior.h>

struct behavior_stepped_scroll_config {
    uint16_t input_code;
};

static int on_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                     struct zmk_behavior_binding_event event) {
    ARG_UNUSED(event);
    const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
    const struct behavior_stepped_scroll_config *config = dev->config;
    input_report_rel(dev, config->input_code, (int16_t)binding->param1, true, K_NO_WAIT);
    return ZMK_BEHAVIOR_OPAQUE;
}

/* No binding_released: dispatcher returns -ENOTSUP, callers discard it. */
static const struct behavior_driver_api behavior_stepped_scroll_api = {
    .binding_pressed = on_keymap_binding_pressed,
};

#define STEPPED_SCROLL_INST(instance)                                                              \
    static const struct behavior_stepped_scroll_config                                             \
        behavior_stepped_scroll_config_##instance = {                                              \
            .input_code = (uint16_t)DT_INST_PROP(instance, input_code),                            \
        };                                                                                         \
    BEHAVIOR_DT_INST_DEFINE(instance, NULL, NULL, NULL,                                            \
                            &behavior_stepped_scroll_config_##instance,                            \
                            POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                      \
                            &behavior_stepped_scroll_api);

DT_INST_FOREACH_STATUS_OKAY(STEPPED_SCROLL_INST)
