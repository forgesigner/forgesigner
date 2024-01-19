import numpy as np
import matplotlib.pyplot as plt


output_filepath = "onnx_model_raw_output.txt"

data = []
with open(output_filepath) as file:
    for line in file:
        data += list(map(float, line.split()))

data = np.array(data)

# Assume onnx uses the same conventional layout that numpy uses
# Namely, lets assume that ouput has shape (batch_size, image_height, image_width)
# typical for PyTorch, see e.g. https://pytorch.org/docs/stable/generated/torch.nn.Conv2d.html
# Here we have image_height=936 and image_width=662, and no batch dimension (batch_size=1)
# Let h be height index (vertical, Y) and w be width index (horizontal, X).
# Then img[batch_idx, h, w] ==
#   data[batch_idx * image_height * image_width + h * image_width + w]
img = data.reshape(936, 662)

# Verify the above described reshaping convention
image_height = 936
image_width = 662
assert img.shape == (image_height, image_width)
for h in range(image_height):
    for w in range(image_width):
        assert img[h, w] == data[h * image_width + w]

# Single pixel was marked as 1, others are zero.
# See create_onnxcheckpoint_for_single_pixel_model.py
assert img[935, 661] == 1
assert (img != 0.).sum() == 1

# Make sure the marked pixel is indeed there
plt.imshow(img, cmap='gray')
plt.colorbar()
plt.show()
