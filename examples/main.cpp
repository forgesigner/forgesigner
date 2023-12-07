#include <onnxruntime/onnxruntime_cxx_api.h>

#include <array>
#include <numeric>

int main() {
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "ONNXRuntimeModel");

    const std::string model_path = "best_checkpoint.onnx";

    Ort::SessionOptions session_options;
    Ort::Session onnx_session(env, model_path.c_str(), session_options);

    // // Prepare input tensor (replace with your actual input data)
    // std::vector<float> input_data = { /* your input data here */ };
    // Ort::Value input_tensor = Ort::Value::CreateTensor<float>(env, input_data.data(), input_data.size(), {1, 3, 936, 662});

    auto memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);

    auto shape = std::array<int64_t, 4>{1, 3, 936, 662};
    int64_t size = 3 * 936 * 662; // std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<int64_t>());
    std::array<float, size> input{};
    std::array<float, size> output{};

    const char* input_names[] = { "input" };
    const char* output_names[] = { "output" };
    Ort::RunOptions run_options{};

    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(memory_info, input.data(), 1, shape.data(), 4);
    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(memory_info, input.data(), 1, shape.data(), 4);

    onnx_session.Run(run_options, input_names, &input_tensor_, 1, output_names, &output_tensor_, 1);

    return 0;
}
