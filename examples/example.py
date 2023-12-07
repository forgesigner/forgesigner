import torch
import onnx
import onnxruntime

import matplotlib.pyplot as plt

onnx_model = onnx.load("best_checkpoint.onnx") #, operator_export_type=torch.onnx.OperatorExportTypes.ONNX_ATEN_FALLBACK)
# onnx.checker.check_model(onnx_model)

x = torch.randn(1, 3, 936, 662, requires_grad=True)

ort_session = onnxruntime.InferenceSession("best_checkpoint.onnx", providers=["CPUExecutionProvider"])

def to_numpy(tensor):
    return tensor.detach().cpu().numpy() if tensor.requires_grad else tensor.cpu().numpy()

# compute ONNX Runtime output prediction
ort_inputs = {ort_session.get_inputs()[0].name: to_numpy(x)}
ort_outs = ort_session.run(None, ort_inputs)

print(ort_outs[0].shape)

image = ort_outs[0][0][0]
plt.imshow(image, cmap='gray')  # 'gray' colormap is suitable for 2D intensity-like images
plt.colorbar()  # Optionally, add a colorbar to show the intensity values
plt.show()
