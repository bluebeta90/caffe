/*
All modification made by Intel Corporation: © 2016 Intel Corporation

All contributions by the University of California:
Copyright (c) 2014, 2015, The Regents of the University of California (Regents)
All rights reserved.

All other contributions:
Copyright (c) 2014, 2015, the respective contributors
All rights reserved.
For the list of contributors go to https://github.com/BVLC/caffe/blob/master/CONTRIBUTORS.md


Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/



#ifndef CAFFE_SMOOTH_L1_LOSS_OHEM_LAYER_HPP_
#define CAFFE_SMOOTH_L1_LOSS_OHEM_LAYER_HPP_

#include <vector>

#include "caffe/blob.hpp"
#include "caffe/common.hpp"
#include "caffe/layer.hpp"
#include "caffe/proto/caffe.pb.h"

#include "caffe/layers/loss_layer.hpp"

namespace caffe {

/**
 * @brief SmoothL1LossOHEMLayer
 *
 * R-FCN
 * Written by Yi Li
 */
  template <typename Dtype>
  class SmoothL1LossOHEMLayer : public LossLayer<Dtype> {
   public:
    explicit SmoothL1LossOHEMLayer(const LayerParameter& param)
      : LossLayer<Dtype>(param), diff_() {}
    virtual void LayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
    virtual void Reshape(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);

    virtual inline const char* type() const { return "SmoothL1LossOHEM"; }

    virtual inline int ExactNumBottomBlobs() const { return -1; }
    virtual inline int MinBottomBlobs() const { return 2; }
    virtual inline int MaxBottomBlobs() const { return 3; }
    virtual inline int ExactNumTopBlobs() const { return -1; }
    virtual inline int MinTopBlobs() const { return 1; }
    virtual inline int MaxTopBlobs() const { return 2; }

    /**
    * Unlike most loss layers, in the SmoothL1LossOHEMLayer we can backpropagate
    * to both inputs -- override to return true and always allow force_backward.
    */
    virtual inline bool AllowForceBackward(const int bottom_index) const {
      return true;
    }

   protected:
    virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
    virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);

    virtual void Backward_cpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);
    virtual void Backward_gpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);

    /// Read the normalization mode parameter and compute the normalizer based
    /// on the blob size.
    virtual Dtype get_normalizer(
      LossParameter_NormalizationMode normalization_mode,
      Dtype pre_fixed_normalizer);

    Blob<Dtype> diff_;
    Blob<Dtype> errors_;
    bool has_weights_;

    int outer_num_, inner_num_;

    /// How to normalize the output loss.
    LossParameter_NormalizationMode normalization_;
  };

}  // namespace caffe

#endif  // CAFFE_SMOOTH_L1_LOSS_OHEM_LAYER_HPP_
