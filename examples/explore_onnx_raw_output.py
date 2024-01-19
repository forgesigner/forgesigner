import numpy as np
import matplotlib.pyplot as plt


output_filepath = "onnx_model_raw_output.txt"

data = []
with open(output_filepath) as file:
    for line in file:
        data += list(map(float, line.split()))

data = np.array(data)

# Assume onnx uses the same conventional layout that numpy uses
img = data.reshape(936, 662)

plt.imshow(img, cmap='gray')
plt.colorbar()
plt.show()
