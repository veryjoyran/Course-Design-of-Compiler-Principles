#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <bits/stdc++.h>
#include "grammar.h"
#include "lexer.h"
#include "parser.h"
#include "symbolTable.h"
#include "optimizequa.h"
#include "lexical.h"
#include "complier.h"

std::ifstream infile_symTable("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/symbolTable_in.txt");


// Function declarations
void test();
std::string readOutputFile(const std::string& filepath);

// int main() {
//    // test();
// //    lexical_test();
// //     grammar_test();
// //     lexer_test();
//     // parser_test();
//      run();


//     return 0;
// }


//Main code
int main(int, char**)
{
    // Setup window
    if (!glfwInit())
        return -1;

    const char* glsl_version = "#version 130";
    GLFWwindow* window = glfwCreateWindow(1280, 720, "编译器测试界面", NULL, NULL);
    if (window == NULL)
        return -1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
    bool err = glewInit() != GLEW_OK;
    if (err) {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return -1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Use default font
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Main loop
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    std::string outputContent;

    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show a simple window
        {
            ImGui::Begin("Course-Design-of-Compiler-Principles");
            if (ImGui::Button("start test")) {
                test();
                outputContent = readOutputFile("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/out.txt");
            }
            // if (ImGui::Button("Lexical Test")) {
            //     lexical_test();
            //     outputContent = readOutputFile("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/lexical_out.txt");
            // }
            // ImGui::SameLine();
            // if (ImGui::Button("Grammar Test")) {
            //     grammar_test();
            //     outputContent = readOutputFile("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/grammar_out.txt");
            // }
            // ImGui::SameLine();
            // if (ImGui::Button("Lexer Test")) {
            //     lexer_test();
            //     outputContent = readOutputFile("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/lexer_out.txt");
            // }
            // ImGui::SameLine();
            // if (ImGui::Button("SymbolTable Test")) {
            //     run();
            //     outputContent = readOutputFile("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/parser_out.txt");
            // }
            
            ImGui::Text("Output:");
            ImGui::TextWrapped("%s", outputContent.c_str());
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void test() {
    std::ifstream infile("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/in.txt");
    std::ofstream outfile("/home/joyran/Course-Design-of-Compiler-Principles/in_out_txt/out.txt");

    if (!infile.is_open() || !outfile.is_open()) {
        std::cerr << "Error opening files!" << std::endl;
    }

    std::streambuf* cinbuf = std::cin.rdbuf();
    std::cin.rdbuf(infile.rdbuf());
    std::streambuf* coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(outfile.rdbuf());

    Complier c;
    c.Init_Gram();
    std::cout << "Grammar Init Succsess\n\n";

    c.parser.Get_Action();
    std::cout << "Action_Init Success\n\n";
    
    std::string sourceCode((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
    c.origin = sourceCode;
    
    std::cin.rdbuf(cinbuf);
    std::string line;
    while (std::getline(infile_symTable, line)) {
        c.symtbl.lines.push_back(line);
    }
    c.lines = c.symtbl.lines;
    
    c.Lexical_Analysis();
    std::cout << "Lexer Succsess\n\n";

    c.Grammatical_Analysis();
    std::cout << "LL(1) Success\n\n";

    c.Init_SymTable();
    std::cout << "SymbolTable Success\n\n";

    c.Get_Quats();
    std::cout << "Quats Success\n\n";

    c.Init_opt();
    c.opt.run();

    std::cout.rdbuf(coutbuf);

    std::cout << "All tasks completed!!!" << std::endl;
    
    infile.close();
    outfile.close();
}

std::string readOutputFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return "Error opening output file!";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
