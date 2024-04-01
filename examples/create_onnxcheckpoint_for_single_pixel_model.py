import torch
import numpy as np
import torch.nn as nn
import torchvision.models as models
import torch.nn.functional as F
import torch.nn.functional as F


def _make_deconv_layer(num_layers):
    layers = []
    in_channels = 512
    out_channels = 256
    for i in range(num_layers):
        layers.append(
            nn.ConvTranspose2d(
                in_channels,
                out_channels,
                kernel_size=4,
                stride=2,
                padding=1,
                output_padding=0,
                bias=False,
            )
        )
        layers.append(nn.BatchNorm2d(out_channels))
        layers.append(nn.ReLU(inplace=True))
        in_channels = out_channels
        out_channels = out_channels // 2

    return nn.Sequential(*layers)


class SignatureCenterNet(nn.Module):
    def __init__(self, target_height=936, target_width=662):
        super(SignatureCenterNet, self).__init__()
        self.backbone = models.resnet18(pretrained=True)
        self.backbone = nn.Sequential(*(list(self.backbone.children())[:-2]))
        self.deconv_layers = _make_deconv_layer(3)
        self.final_layer = nn.Conv2d(
            in_channels=64, out_channels=1, kernel_size=1, stride=1, padding=0
        )

        # self.adaptive_pool = nn.Av

    def forward(self, x):
        x = self.backbone(x)
        x = self.deconv_layers(x)
        heatmap = self.final_layer(x)
        heatmap = F.interpolate(heatmap, size=(936, 662), mode='bilinear', align_corners=False)
        return heatmap


class SinglePixelModel(SignatureCenterNet):
    """
    A wrapper around SignatureCenterNet that returns tensor of
    the appropriate shape filled with zeros except for one pixel.
    Motivation: save this model as "*.onnx" checkpoint, load it in C++, run
    inference with Onnx Ort C++ API, save the raw output tensor
    and see how it's aligned knowing that (x,y) pixel has to be 1.
    """
    def __init__(self, single_pixel_x: int, single_pixel_y: int, *args, **kwargs):
        self.single_pixel_x = single_pixel_x 
        self.single_pixel_y = single_pixel_y 
        super().__init__(*args, **kwargs)

    def forward(self, x):
        heatmap = super().forward(x)
        heatmap = heatmap * 0
        heatmap[..., self.single_pixel_x, self.single_pixel_y] = 1
        return heatmap


if __name__ == "__main__":
    bottom_right_pixel_model = SinglePixelModel(935, 661)

    dummy_input = torch.randn(1, 3, 936, 662)
    torch.onnx.export(bottom_right_pixel_model,
                      dummy_input,
                      "bottom_right_pixel_model.onnx")
