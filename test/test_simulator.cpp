//
//  main.cpp
//

#include <iostream>
#include "DisplayOpenGL3D.h"
#include "OmniDirectionalController.h"
#include "DifferentialDriveController.h"
#include "Simulator.h"
#include "SimpleUnit.h"
#include <cmath>
#include <thread>
#include <future>
#include <chrono>

#define GL_SILENCE_DEPRECATION

int xSize = 600;
int ySize = 600;

float cameraYaw = 220.0f;
float cameraPitch = -60.0f;

float cameraYawMemo = cameraYaw;
float cameraPitchMemo = cameraPitch;

float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZ = 0.0f;
float zoom = 0.1f;
bool isMouseLeftButtonPressed = false;
bool isTPressed = false;
int mouseX, mouseY;

float speed = 0.5;
float space_tolerance = 0.05;
float orientation_tolerance = 0.2;
int frequency = 60;
long period = 1000 / frequency;

std::shared_ptr<IDisplay> display_interface = std::make_shared<DisplayOpenGL3D>();
std::shared_ptr<ISimulator> simulator = std::make_shared<Simulator>(display_interface);

void mouseButtonCallback(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            isMouseLeftButtonPressed = true;
            mouseX = x;
            mouseY = y;
        }
        else if (state == GLUT_UP)
        {
            isMouseLeftButtonPressed = false;
        }
    }
}

void mouseMotionCallback(int x, int y)
{

    if (isMouseLeftButtonPressed)
    {

        if (isTPressed)
        {
            float originX = xSize / 2.0;
            float originY = ySize / 2.0;

            float distMouseOriginX = (x - originX) / xSize * 0.05;
            float distMouseOriginY = (y - originY) / ySize * 0.05;

            cameraX += distMouseOriginX * zoom;
            cameraY -= distMouseOriginY * zoom;
        }

        else
        {
            int deltaX = x - mouseX;
            int deltaY = y - mouseY;

            cameraYaw += deltaX * 0.1f;
            cameraPitch += deltaY * 0.1f;

            if (cameraPitch > 179.0f)
                cameraPitch = 179.0f;
            if (cameraPitch < -179.0f)
                cameraPitch = -179.0f;

            mouseX = x;
            mouseY = y;
        }

        glutPostRedisplay();
    }
}

void mouseWheelCallback(int wheel, int direction, int x, int y)
{
    if (wheel == 0)
    {
        if (direction > 0)
        {

            zoom += 0.01;
        }
        else
        {
            if (zoom > 0.02)
            {
                zoom -= 0.01;
            }
        }
        glutPostRedisplay();
    }
}

void keyboardCallback(unsigned char key, int x, int y)
{
    if (isTPressed == false && key == 't')
    {
        cameraPitchMemo = cameraPitch;
        cameraYawMemo = cameraYaw;

        cameraPitch = 0.0;
        cameraYaw = 360;

        isTPressed = true;
    }
    else if (isTPressed == true && key == 't')
    {
        isTPressed = false;

        cameraPitch = cameraPitchMemo;
        cameraYaw = cameraYawMemo;
    }
}

void init_display()
{
    display_interface->init_display(cameraX, cameraY, cameraZ);
}

void display()
{
    display_interface->update_camera(cameraPitch, cameraYaw, cameraX, cameraY, cameraZ, zoom);
    display_interface->addGeometry(Reference(-1, Point(-cameraX, -cameraY, cameraZ), 0.1), false);
    display_interface->display();
    display_interface->removeGeometry(-1, false);
}

void display_idle(int value)
{
    glutTimerFunc(50, display_idle, 0);
    if (display_interface->has_been_updated())
    {
        display();
    }
}

int main(int argc, const char *argv[])
{

    std::vector<std::string> argv_copy(argv, argv + argc);
    std::vector<char *> argv_nc;
    for (std::string &str : argv_copy)
    {
        argv_nc.push_back(&str[0]);
    }

    glutInit(&argc, argv_nc.data());
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(xSize, ySize);
    glutCreateWindow("OpenGL Window");

    glutMouseFunc(mouseButtonCallback);
    glutMotionFunc(mouseMotionCallback);
    glutMouseWheelFunc(mouseWheelCallback);
    glutKeyboardFunc(keyboardCallback);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0);

    glEnable(GL_DEPTH_TEST);
    init_display();
    glutDisplayFunc(display);

    std::shared_ptr<IController> controller = std::make_shared<OmniDirectionalController>();
    // std::shared_ptr<IController> controller = std::make_shared<DifferentialDriveController>();

    std::shared_ptr<IUnit> unitPtr = std::make_shared<SimpleUnit>("u_one", Point(0.0, 0.0, 0.0), Orientation(1.2, 0.0, 0.1), controller, speed, space_tolerance, orientation_tolerance);
    simulator->add_unit(unitPtr);
    std::vector<Point> unitOneGoals = {Point(1.0, 0.0, 0.0), Point(1.0, 1.0, 0.0), Point(0.0, 1.0, 1.0), Point(0.0, 0.0, 0.0)};
    std::vector<Orientation> unitOneGoalsOrientation = {Orientation(0.0, 0.0, 0.0), Orientation(1.0, 1.0, 0.0), Orientation(0.0, 0.0, 2.0), Orientation(-1.0, 0.0, 1.57)};
    int unit_one_current_path_idx = 0;
    unitPtr->go_to(unitOneGoals[unit_one_current_path_idx], Orientation(0.0, 0.0, 0.0));

    std::shared_ptr<IUnit> unitPtrTwo = std::make_shared<SimpleUnit>("u_two", Point(0.1, 0.0, 0.0), Orientation(1.57, 3.2, 1.57), controller, speed, space_tolerance, orientation_tolerance);
    simulator->add_unit(unitPtrTwo);
    std::vector<Point> unitTwoGoals = {Point(0.0, 1.0, 0.0), Point(1.0, 1.0, 0.0), Point(1.0, 0.0, 0.0), Point(0.0, 0.0, 0.0)};
    std::vector<Orientation> unitTwoGoalsOrientation = {Orientation(0.0, 0.0, 0.0), Orientation(1.0, 1.0, 0.0), Orientation(0.0, 0.0, 2.0), Orientation(-1.0, 0.0, 1.57)};
    int unit_two_current_path_idx = 0;
    unitPtrTwo->go_to(unitTwoGoals[unit_two_current_path_idx], Orientation(0.0, 0.0, 0.0));

    long periodMain = period;
    std::thread computeThread([&periodMain]()
                              {
        while (true) {
            simulator->compute();
            display_interface->update();
            std::this_thread::sleep_for(std::chrono::milliseconds(periodMain));
        } });

    std::thread threadUnitOne([&unitPtr, &unitOneGoals, &unit_one_current_path_idx, &unitOneGoalsOrientation]()
                              {
        while (true && unit_one_current_path_idx != unitOneGoals.size()-1) {
            
            if(unitPtr->is_arrived(false)){
                unit_one_current_path_idx += 1;
                std::cout << "current point is :" << unit_one_current_path_idx << std::endl;
                unitPtr->go_to(unitOneGoals[unit_one_current_path_idx], unitOneGoalsOrientation[unit_one_current_path_idx]);
                display_interface->removeGeometry(1000, true);
                display_interface->addGeometry(Sphere(1000, unitOneGoals[unit_one_current_path_idx], 0.01), true); 
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        } });

    std::thread threadUnitTwo([&unitPtrTwo, &unitTwoGoals, &unit_two_current_path_idx, &unitTwoGoalsOrientation]()
                              {
        while (true && unit_two_current_path_idx != unitTwoGoals.size()-1) {
            if(unitPtrTwo->is_arrived(false)){
                unit_two_current_path_idx += 1;
                unitPtrTwo->go_to(unitTwoGoals[unit_two_current_path_idx], unitTwoGoalsOrientation[unit_two_current_path_idx]);
                display_interface->removeGeometry(1000, true);
                display_interface->addGeometry(Sphere(1000, unitTwoGoals[unit_two_current_path_idx], 0.01), true);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        } });

    glutTimerFunc(50, display_idle, 0);
    glutMainLoop();

    computeThread.join();

    return 0;
}