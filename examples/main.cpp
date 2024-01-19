#include <onnxruntime/onnxruntime_cxx_api.h>
// #include <onnxruntime/experimental_onnxruntime_cxx_api.h>

#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <numeric>

void print_output_to_file(const auto& data, const std::string& file_path) {
    std::ofstream output_file(file_path);
    if (!output_file.is_open()) {
        std::cerr << "Error opening file: " << file_path << std::endl;
        return;
    }

    for (const auto& value : data) {
        output_file << value << " ";
    }
}

int main() {
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "ONNXRuntimeModel");

    const std::string model_path = "best_checkpoint.onnx";

    Ort::SessionOptions session_options;
    Ort::Session session(env, model_path.c_str(), session_options);

    auto memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);

    constexpr auto input_shape = std::array<int64_t, 4>{1, 3, 936, 662};
    const char* const input_names[] = {"input.1"};
    constexpr int64_t input_size = std::accumulate(input_shape.begin(), input_shape.end(), 1, std::multiplies<int64_t>());
    std::vector<float> raw_input(input_size, 0);
    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
        memory_info, raw_input.data(), raw_input.size(), input_shape.data(), input_shape.size()
    );

    constexpr auto output_shape = std::array<int64_t, 4>{1, 1, 936, 662};
    constexpr int64_t output_size = std::accumulate(output_shape.begin(), output_shape.end(), 1, std::multiplies<int64_t>());
    std::vector<float> raw_output(output_size, 0);
    // following has to be possible but doesn't work: const auto names = session.GetOutputNames();
    const char* const output_names[] = {"227"};
    Ort::Value output_tensor = Ort::Value::CreateTensor<float>(
        memory_info, raw_output.data(), raw_output.size(), output_shape.data(), output_shape.size()
    );

    Ort::RunOptions run_options{};
    constexpr size_t input_count = 1;
    constexpr size_t output_count = 1;
    session.Run(run_options, input_names, &input_tensor, input_count,
                             output_names, &output_tensor, output_count);
    
    // Write tensor to disk, then examine the output visually in Python, for simplicity
    print_output_to_file(raw_output, "onnx_model_raw_output.txt");
}
