import torch
import onnx
import onnxruntime

import matplotlib.pyplot as plt

onnx_model = onnx.load("best_checkpoint.onnx") #, operator_export_type=torch.onnx.OperatorExportTypes.ONNX_ATEN_FALLBACK)
onnx.checker.check_model(onnx_model)

x = torch.randn(1, 3, 936, 662, requires_grad=False)

ort_session = onnxruntime.InferenceSession("best_checkpoint.onnx", providers=["CPUExecutionProvider"])
# help(ort_session)

def to_numpy(tensor):
    return tensor.detach().cpu().numpy() if tensor.requires_grad else tensor.cpu().numpy()

# compute ONNX Runtime output prediction
assert len(ort_session.get_inputs()) == 1
input_name = ort_session.get_inputs()[0].name
print(f"input name: '{input_name}'")

assert len(ort_session.get_outputs()) == 1
output_name = ort_session.get_outputs()[0].name
print(f"ouput name: '{output_name}'")

input_array = to_numpy(x)
ort_inputs = { input_name: input_array }
ort_outs = ort_session.run(None, ort_inputs)

print(ort_outs[0].shape, flush=True)

image = ort_outs[0][0][0]
plt.imshow(image, cmap='gray')
plt.colorbar()
plt.show()
