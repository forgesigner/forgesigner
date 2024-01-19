// https://lindevs.com/get-model-input-and-output-shapes-using-onnx-runtime-and-cpp

#include <iostream>
#include <onnxruntime/onnxruntime_cxx_api.h>

std::ostream &operator<<(std::ostream &os, const std::vector<int64_t> &values) {
    os << "[" << values.front();
    for (int i = 1; i < values.size(); ++i) {
        os << ", " << values[i];
    }
    os << "]";

    return os;
}

int main() {
    Ort::AllocatorWithDefaultOptions allocator;
    Ort::SessionOptions sessionOptions;
    sessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);

    Ort::Session session(Ort::Env(), "bottom_right_pixel_model.onnx", sessionOptions);

    const size_t inputCount = session.GetInputCount();
    for (int i = 0; i < inputCount; ++i) {
        auto name = session.GetInputNameAllocated(i, allocator);
        auto shape = session.GetInputTypeInfo(i).GetTensorTypeAndShapeInfo().GetShape();

        std::cout << "Input Number: " << i << std::endl;
        std::cout << " Input Name: " << name.get() << std::endl;
        std::cout << " Input Shape: " << shape << std::endl;
    }

    size_t outputCount = session.GetOutputCount();
    for (int i = 0; i < outputCount; ++i) {
        auto name = session.GetOutputNameAllocated(i, allocator);
        auto shape = session.GetOutputTypeInfo(i).GetTensorTypeAndShapeInfo().GetShape();

        std::cout << "Output Number: " << i << std::endl;
        std::cout << " Output Name: " << name.get() << std::endl;
        std::cout << " Output Shape: " << shape << std::endl;
    }
}
