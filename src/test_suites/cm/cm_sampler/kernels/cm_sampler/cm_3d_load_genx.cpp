/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

using data_t = READ_TYPE;

extern "C" _GENX_MAIN_ void kernel(SurfaceIndex out [[type("buffer_t")]],
                                   SurfaceIndex image [[type("image2d_t")]],
                                   SurfaceIndex ubuf [[type("buffer_t")]],
                                   SurfaceIndex vbuf [[type("buffer_t")]]) {
  constexpr int simd = SIMD;
  constexpr int channels = CHANNELS;
  constexpr auto channel_mask = CHANNEL_MASK;

  matrix<data_t, channels, simd> result = 0;

  vector<unsigned, simd> u = 0;
  vector<unsigned, simd> v = 0;

  read(ubuf, 0, u);
  read(vbuf, 0, v);

  cm_3d_load<CM_3D_LOAD_LZ, channel_mask>(result.template format<data_t>(), 0,
                                          image, u, v);

#pragma unroll
  for (int i = 0; i < channels; i++) {
    write(out, i * simd * sizeof(float), result.row(i));
  }
}
