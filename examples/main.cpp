#include <onnxruntime/onnxruntime_cxx_api.h>
// #include <onnxruntime/experimental_onnxruntime_cxx_api.h>

#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <numeric>

void printOutputToFile(const auto& data, const std::string& file_path) {
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

    const std::string modelPath = "bottom_right_pixel_model.onnx";

    Ort::Session session(env, modelPath.c_str(), Ort::SessionOptions{});

    auto memoryInfo = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);

    constexpr auto inputShape = std::array<int64_t, 4>{1, 3, 936, 662};
    const char* const inputNames[] = {"input.1"};
    constexpr int64_t inputSize = std::accumulate(inputShape.begin(), inputShape.end(), 1, std::multiplies<int64_t>());
    std::vector<float> rawInput(inputSize, 0);
    Ort::Value inputTensor = Ort::Value::CreateTensor<float>(
        memoryInfo, rawInput.data(), rawInput.size(), inputShape.data(), inputShape.size()
    );

    constexpr auto outputShape = std::array<int64_t, 4>{1, 1, 936, 662};
    constexpr int64_t outputSize = std::accumulate(outputShape.begin(), outputShape.end(), 1, std::multiplies<int64_t>());
    std::vector<float> rawOutput(outputSize, 0);
    const char* const outputNames[] = {"293"};
    Ort::Value outputTensor = Ort::Value::CreateTensor<float>(
        memoryInfo, rawOutput.data(), rawOutput.size(), outputShape.data(), outputShape.size()
    );

    constexpr size_t inputCount = 1;
    constexpr size_t outputCount = 1;
    session.Run(Ort::RunOptions{}, inputNames, &inputTensor, inputCount,
                             outputNames, &outputTensor, outputCount);
    
    // Write tensor to disk, then examine the output visually in Python, for simplicity
    printOutputToFile(rawOutput, "onnx_model_raw_output.txt");
}
