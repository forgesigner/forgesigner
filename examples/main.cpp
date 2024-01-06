#include <onnxruntime/onnxruntime_cxx_api.h>
// #include <onnxruntime/experimental_onnxruntime_cxx_api.h>

#include <iostream>
#include <array>
#include <vector>
#include <numeric>

int main() {
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "ONNXRuntimeModel");

    const std::string model_path = "best_checkpoint.onnx";

    Ort::SessionOptions session_options;
    // Ort::Experimental::Session session(env, model_path.c_str(), session_options);
    Ort::Session session(env, model_path.c_str(), session_options);

    // // Prepare input tensor (replace with your actual input data)
    // std::vector<float> input_data = { /* your input data here */ };
    // Ort::Value input_tensor = Ort::Value::CreateTensor<float>(env, input_data.data(), input_data.size(), {1, 3, 936, 662});

    auto memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);

    constexpr auto input_shape = std::array<int64_t, 4>{1, 3, 936, 662};
    constexpr auto output_shape = std::array<int64_t, 4>{1, 1, 936, 662};
    constexpr int64_t size = 3 * 936 * 662;  // std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<int64_t>());
    std::vector<float> input(size, 0);
    std::vector<float> output(size, 0);

    const char* input_names[] = { "input.1" };
    // const auto names = session.GetOutputNames();
    const char* output_names[] = { "227" };
    Ort::RunOptions run_options{};

    std::cout << "safe till now" << std::endl;
    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(memory_info, input.data(), input.size(), input_shape.data(), input_shape.size());
    Ort::Value output_tensor = Ort::Value::CreateTensor<float>(memory_info, output.data(), output.size(), output_shape.data(), output_shape.size());

    session.Run(run_options, input_names, &input_tensor, 1, output_names, &output_tensor, 1);
}
