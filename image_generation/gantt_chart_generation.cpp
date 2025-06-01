#include <bits/stdc++.h>
#include "../operation.h"
#include "gantt_chart_generation.h"

// CImg library for visualization
#define cimg_display 0    // no display, just save files
#include "CImg.h"         // include CImg.h header

using namespace std;
using namespace chrono;
using namespace cimg_library; // for CImg

template <typename T>
using Matrix = vector<vector<T>>;

// colors for Gantt chart
struct RGB {
    unsigned char r, g, b;
    RGB(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b) {}
};

vector<RGB> color_palette = {
    {255, 0, 0},    // Vermelho
    {0, 255, 0},    // Verde
    {0, 0, 255},    // Azul
    {255, 255, 0},  // Amarelo
    {255, 0, 255},  // Magenta
    {0, 255, 255},  // Ciano
    {128, 0, 0},    // Marrom
    {0, 128, 0},    // Verde escuro
    {0, 0, 128}     // Azul escuro
};

// performance tuning parameters
const int max_propagation_passes = 10; 
const bool use_first_improvement = true; // Switch to first improvement strategy


// color definitions for CImg
const unsigned char black[] = {0, 0, 0};


// function to generate Gantt chart as PNG
void generate_gantt_chart(const Matrix<int>& schedule, int iteration, int makespan, const string& prefix) {
    const int job_height = 40;
    const int time_scale = 30;  // pixels per time unit
    const int chart_width = makespan * time_scale + 100;
    const int chart_height = num_jobs * job_height + 80;
    const int left_margin = 60;
    const int top_margin = 40;
    
    // create an image with white background
    CImg<unsigned char> img(chart_width, chart_height, 1, 3, 255);
    
    // draw time axis
    for (int t = 0; t <= makespan; t += 1) {
        int x = left_margin + t * time_scale;
        // draw tick marks
        img.draw_line(x, top_margin - 5, x, top_margin, black);
        
        // add time labels every 5 units
        if (t % 5 == 0) {
            char label[10];
            sprintf(label, "%d", t);
            img.draw_text(x - 5, top_margin - 20, label, black);
        }
    }
    
    // draw job rows with labels
    for (int j = 0; j < num_jobs; ++j) {
        int y = top_margin + j * job_height;
        
        // draw job label
        char job_label[20];
        snprintf(job_label, sizeof(job_label), "Job %d", j + 1);
        img.draw_text(5, y + job_height/2 - 5, job_label, black);
        
        // draw horizontal line separating jobs
        img.draw_line(left_margin, y, chart_width - 20, y, black);
    }
    
    // draw final horizontal line
    img.draw_line(left_margin, top_margin + num_jobs * job_height, 
                 chart_width - 20, top_margin + num_jobs * job_height, black);
    
    // draw operations as colored blocks
    for (int j = 0; j < num_jobs; ++j) {
        for (int op = 0; op < num_machines; ++op) {
            if (schedule[j][op] != -1) {
                int start_time = schedule[j][op];
                int duration = jobs[j][op].duration;
                int machine = jobs[j][op].machine;
                
                // get color based on machine number
                const RGB& color = color_palette[machine % color_palette.size()];
                const unsigned char machine_color[] = {color.r, color.g, color.b};
                
                // calculate position
                int x1 = left_margin + start_time * time_scale;
                int y1 = top_margin + j * job_height + 5;
                int x2 = x1 + duration * time_scale;
                int y2 = y1 + job_height - 10;
                
                // draw operation block
                img.draw_rectangle(x1, y1, x2, y2, machine_color);
                
                // add operation info
                char op_text[20];
                sprintf(op_text, "M%d", machine+1);
                img.draw_text(x1 + 5, y1 + 10, op_text, black);
            }
        }
    }
    
    // add title with makespan info
    char title[100];
    sprintf(title, "Job Shop Schedule - %s %d - Makespan: %d", prefix.c_str(), iteration, makespan);
    img.draw_text(left_margin, 10, title, black);
    
    // add machine color legend
    int legend_x = left_margin;
    int legend_y = chart_height - 30;
    for (int m = 0; m < num_machines; ++m) {
        const RGB& color = color_palette[m % color_palette.size()];
        const unsigned char machine_color[] = {color.r, color.g, color.b};
        
        // draw color box
        int box_size = 15;
        img.draw_rectangle(legend_x, legend_y, legend_x + box_size, legend_y + box_size, machine_color);
        
        // draw label
        char machine_label[20];
        sprintf(machine_label, "M%d", m+1);
        img.draw_text(legend_x + box_size + 5, legend_y + 3, machine_label, black);
        
        legend_x += 60; // move to next legend item
    }
    
    // save image
    ostringstream filename;
    filename << prefix << "_iteration_" << setw(2) << setfill('0') << iteration << ".png";
    img.save(filename.str().c_str());
    std::cout << "Generated Gantt chart: " << filename.str() << endl;
}