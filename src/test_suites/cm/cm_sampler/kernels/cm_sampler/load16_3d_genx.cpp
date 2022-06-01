/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

using data_t = READ_TYPE;

extern "C" _GENX_MAIN_ void kernel(svmptr_t out [[type("svmptr_t")]],
                                   SurfaceIndex image [[type("image3d_t")]],
                                   svmptr_t ubuf [[type("svmptr_t")]],
                                   svmptr_t vbuf [[type("svmptr_t")]],
                                   svmptr_t rbuf [[type("svmptr_t")]]) {
  constexpr int channels = CHANNELS;
  constexpr auto channel_mask = CHANNEL_MASK;

  matrix<data_t, channels, 16> result = 0;

  vector<unsigned, 16> u = 0;
  vector<unsigned, 16> v = 0;
  vector<unsigned, 16> r = 0;

  cm_svm_block_read(ubuf, u);
  cm_svm_block_read(vbuf, v);
  cm_svm_block_read(rbuf, r);

  load16(result, channel_mask, image, u, v, r);

#pragma unroll
  for (int i = 0; i < channels; i++) {
    cm_svm_block_write(out + i * 16 * sizeof(data_t), result.row(i));
  }
}
