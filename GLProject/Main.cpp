#include <iostream>
#include <cmath>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <learnopengl/shader_m.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Polygon.cpp"   

using namespace glm;

vec3 cameraPos = vec3(0.0f, 1.6f, 5.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

const float PI = 3.14159265358979323846f;

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 0.5f;
    const float rotStep = 0.02f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cameraPos += cross(cameraFront, normalize(cross(cameraFront, cameraUp))) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cameraPos -= cross(cameraFront, normalize(cross(cameraFront, cameraUp))) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        mat4 rotationMatrix = glm::rotate(mat4(1.0f), rotStep, vec3(0.0f, 1.0f, 0.0f));
        vec4 rotatedVectorHomogeneous = rotationMatrix * vec4(cameraFront, 1.0f);
        cameraFront = normalize(vec3(rotatedVectorHomogeneous));
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        mat4 rotationMatrix = glm::rotate(mat4(1.0f), -rotStep, vec3(0.0f, 1.0f, 0.0f));
        vec4 rotatedVectorHomogeneous = rotationMatrix * vec4(cameraFront, 1.0f);
        cameraFront = normalize(vec3(rotatedVectorHomogeneous));
    }
}

std::vector<vec3> makeCircle(float cx, float cy, float z, float r, int segments)
{
    std::vector<vec3> pts;
    pts.reserve(segments + 2);
    pts.push_back(vec3(cx, cy, z));
    for (int i = 0; i <= segments; ++i) {
        float a = (float)i / (float)segments * 2.0f * PI;
        pts.push_back(vec3(cx + cosf(a) * r, cy + sinf(a) * r, z));
    }
    return pts;
}

std::vector<vec3> makeQuadAsFan(float xmin, float xmax, float ymin, float ymax, float z)
{
    std::vector<vec3> pts;
    vec3 c((xmin + xmax) * 0.5f, (ymin + ymax) * 0.5f, z);
    pts.push_back(c);
    pts.push_back(vec3(xmin, ymin, z));
    pts.push_back(vec3(xmax, ymin, z));
    pts.push_back(vec3(xmax, ymax, z));
    pts.push_back(vec3(xmin, ymax, z));
    pts.push_back(vec3(xmin, ymin, z));
    return pts;
}

void addBoxAsPolygons(std::vector<Polygon*>& out, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax, vec3 color)
{
    out.push_back(new Polygon(makeQuadAsFan(xmin, xmax, ymin, ymax, zmax), color));
    out.push_back(new Polygon(makeQuadAsFan(xmin, xmax, ymin, ymax, zmin), color));

    {
        std::vector<vec3> v;
        vec3 center(xmin, (ymin + ymax) * 0.5f, (zmin + zmax) * 0.5f);
        v.push_back(center);
        v.push_back(vec3(xmin, ymin, zmin));
        v.push_back(vec3(xmin, ymin, zmax));
        v.push_back(vec3(xmin, ymax, zmax));
        v.push_back(vec3(xmin, ymax, zmin));
        v.push_back(vec3(xmin, ymin, zmin));
        out.push_back(new Polygon(v, color));
    }
    {
        std::vector<vec3> v;
        vec3 center(xmax, (ymin + ymax) * 0.5f, (zmin + zmax) * 0.5f);
        v.push_back(center);
        v.push_back(vec3(xmax, ymin, zmin));
        v.push_back(vec3(xmax, ymin, zmax));
        v.push_back(vec3(xmax, ymax, zmax));
        v.push_back(vec3(xmax, ymax, zmin));
        v.push_back(vec3(xmax, ymin, zmin));
        out.push_back(new Polygon(v, color));
    }
    {
        std::vector<vec3> v;
        vec3 center((xmin + xmax) * 0.5f, ymax, (zmin + zmax) * 0.5f);
        v.push_back(center);
        v.push_back(vec3(xmin, ymax, zmin));
        v.push_back(vec3(xmax, ymax, zmin));
        v.push_back(vec3(xmax, ymax, zmax));
        v.push_back(vec3(xmin, ymax, zmax));
        v.push_back(vec3(xmin, ymax, zmin));
        out.push_back(new Polygon(v, color));
    }
    {
        std::vector<vec3> v;
        vec3 center((xmin + xmax) * 0.5f, ymin, (zmin + zmax) * 0.5f);
        v.push_back(center);
        v.push_back(vec3(xmin, ymin, zmin));
        v.push_back(vec3(xmax, ymin, zmin));
        v.push_back(vec3(xmax, ymin, zmax));
        v.push_back(vec3(xmin, ymin, zmax));
        v.push_back(vec3(xmin, ymin, zmin));
        out.push_back(new Polygon(v, color));
    }
}

int main()
{
    if (!glfwInit()) { std::cerr << "GLFW init failed\n"; return -1; }
    GLFWwindow* window = glfwCreateWindow(900, 700, "3D Pendulum Clock - fixed positions", NULL, NULL);
    if (!window) { std::cerr << "Window create failed\n"; glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { std::cerr << "GLAD init failed\n"; return -1; }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    Shader ourShader("./shaders/vs/L3.vs", "./shaders/fs/L3.fs");

    // Colors (normalized)
    vec3 woodLight = vec3(185.0f, 120.0f, 75.0f) / 255.0f;
    vec3 woodDark = vec3(110.0f, 70.0f, 40.0f) / 255.0f;
    vec3 dialFace = vec3(248.0f, 244.0f, 235.0f) / 255.0f;
    vec3 markColor = vec3(20.0f, 20.0f, 20.0f) / 255.0f;
    vec3 brass = vec3(200.0f, 160.0f, 70.0f) / 255.0f;
    vec3 metalBob = vec3(212.0f, 190.0f, 145.0f) / 255.0f;
    vec3 handCol = vec3(30.0f, 30.0f, 30.0f) / 255.0f;

    const int CSEG = 64;
    float dialCenterY = 1.27f;
    float bezelOuterR = 1.35f;
    float bezelInnerR = 1.08f;

    float caseYmin = -4.0f;
    float caseYmax = dialCenterY - 0.05f;      
    float caseZmin = -0.35f;
    float caseZmax = 0.02f;

    float backZmin = -1.2f;
    float backZmax = -0.6f;

    float shelfYmin = caseYmax;
    float shelfYmax = caseYmax + 0.18f;

    std::vector<Polygon*> backPrism;
    addBoxAsPolygons(backPrism, -4.8f, 4.8f, -6.2f, 6.2f, backZmin, backZmax, woodDark);

    std::vector<Polygon*> casePrism;
    addBoxAsPolygons(casePrism, -1.25f, 1.25f, caseYmin, caseYmax, caseZmin, caseZmax, woodLight);

    std::vector<Polygon*> topShelfPrism;
    addBoxAsPolygons(topShelfPrism, -1.45f, 1.45f, shelfYmin, shelfYmax, caseZmin - 0.1f, caseZmax + 0.08f, woodDark);

    std::vector<Polygon*> bottomLedgePrism;
    addBoxAsPolygons(bottomLedgePrism, -1.45f, 1.45f, caseYmin - 0.15f, caseYmin + 0.05f, caseZmin - 0.1f, caseZmax + 0.02f, woodDark);

    Polygon* BezelBack = new Polygon(makeCircle(0.0f, dialCenterY, 0.00f, bezelOuterR, CSEG), woodDark);
    Polygon* BezelFront = new Polygon(makeCircle(0.0f, dialCenterY, 0.20f, bezelOuterR, CSEG), woodDark);
    Polygon* BrassRing = new Polygon(makeCircle(0.0f, dialCenterY, 0.26f, (bezelOuterR + bezelInnerR) * 0.5f, CSEG), brass);
    Polygon* DialFace = new Polygon(makeCircle(0.0f, dialCenterY, 0.30f, bezelInnerR * 0.98f, CSEG), dialFace);
    Polygon* Hub = new Polygon(makeCircle(0.0f, dialCenterY, 0.33f, 0.06f, CSEG), vec3(30.0f / 255.0f));

    std::vector<vec3> baseMarkGeom = makeQuadAsFan(-0.04f, 0.04f, dialCenterY - 0.95f, dialCenterY - 0.68f, 0.32f);
    std::vector<Polygon*> Marks;
    for (int i = 0; i < 12; ++i) Marks.push_back(new Polygon(baseMarkGeom, markColor));

    std::vector<vec3> hourVerts = {
        {-0.06f, dialCenterY - 0.06f, 0.34f}, {0.06f, dialCenterY - 0.06f, 0.34f}, {0.06f, dialCenterY + 0.28f, 0.34f}, {-0.06f, dialCenterY + 0.28f, 0.34f},
        {-0.06f, dialCenterY - 0.06f, 0.31f}, {0.06f, dialCenterY - 0.06f, 0.31f}, {0.06f, dialCenterY + 0.28f, 0.31f}, {-0.06f, dialCenterY + 0.28f, 0.31f},
    };
    Polygon* HandHour = new Polygon(hourVerts, handCol);

    std::vector<vec3> minVerts = {
        {-0.045f, dialCenterY - 0.06f, 0.36f}, {0.045f, dialCenterY - 0.06f, 0.36f}, {0.045f, dialCenterY + 0.48f, 0.36f}, {-0.045f, dialCenterY + 0.48f, 0.36f},
        {-0.045f, dialCenterY - 0.06f, 0.31f}, {0.045f, dialCenterY - 0.06f, 0.31f}, {0.045f, dialCenterY + 0.48f, 0.31f}, {-0.045f, dialCenterY + 0.48f, 0.31f},
    };
    Polygon* HandMin = new Polygon(minVerts, handCol);

    std::vector<vec3> secVerts = {
        {-0.02f, dialCenterY - 0.06f, 0.37f}, {0.02f, dialCenterY - 0.06f, 0.37f}, {0.02f, dialCenterY + 0.6f, 0.37f}, {-0.02f, dialCenterY + 0.6f, 0.37f},
        {-0.02f, dialCenterY - 0.06f, 0.31f}, {0.02f, dialCenterY - 0.06f, 0.31f}, {0.02f, dialCenterY + 0.6f, 0.31f}, {-0.02f, dialCenterY + 0.6f, 0.31f},
    };
    Polygon* HandSec = new Polygon(secVerts, vec3(180.0f / 255.0f, 30.0f / 255.0f, 30.0f / 255.0f));

    Polygon* PendulumShaft = new Polygon(makeQuadAsFan(-0.02f, 0.02f, -1.8f - 1.0f, dialCenterY - 0.35f, 0.28f), vec3(80.0f / 255.0f, 50.0f / 255.0f, 20.0f / 255.0f));
    Polygon* PendulumBob = new Polygon(makeCircle(0.0f, -2.8f, 0.40f, 0.32f, CSEG), metalBob);

    ourShader.use();
    ourShader.setMat4("projection", perspective(radians(45.0f), 900.0f / 700.0f, 0.1f, 100.0f));

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.88f, 0.94f, 0.98f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4 view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        ourShader.setMat4("view", view);

        mat4 I = mat4(1.0f);

        for (auto p : backPrism) { p->transformation(I); p->draw(ourShader); }

        for (auto p : casePrism) { p->transformation(I); p->draw(ourShader); }
        for (auto p : topShelfPrism) { p->transformation(I); p->draw(ourShader); }
        for (auto p : bottomLedgePrism) { p->transformation(I); p->draw(ourShader); }

        BezelBack->transformation(I); BezelBack->draw(ourShader);
        BezelFront->transformation(I); BezelFront->draw(ourShader);
        BrassRing->transformation(I);  BrassRing->draw(ourShader);
        DialFace->transformation(I);   DialFace->draw(ourShader);

        for (int i = 0; i < 12; ++i) {
            float ang = i * (2.0f * PI / 12.0f);
            mat4 T = translate(mat4(1.0f), vec3(0.0f, dialCenterY, 0.0f));
            T = rotate(T, -ang, vec3(0, 0, 1));
            T = translate(T, vec3(0.0f, -dialCenterY, 0.0f));
            Marks[i]->transformation(T);
            Marks[i]->draw(ourShader);
        }
        //time 
        float t = (float)glfwGetTime();

        float secAngle = fmod(t, 60.0f) * (2.0f * PI / 60.0f);
        mat4 secT = translate(mat4(1.0f), vec3(0.0f, dialCenterY, 0.0f));
        secT = rotate(secT, -secAngle, vec3(0, 0, 1));
        secT = translate(secT, vec3(0.0f, -dialCenterY, 0.0f));
        HandSec->transformation(secT); HandSec->draw(ourShader);

        float minuteAngle = -(150.0f * PI / 180.0f);
        mat4 minT = translate(mat4(1.0f), vec3(0.0f, dialCenterY, 0.0f));
        minT = rotate(minT, minuteAngle, vec3(0, 0, 1));
        minT = translate(minT, vec3(0.0f, -dialCenterY, 0.0f));
        HandMin->transformation(minT); HandMin->draw(ourShader);

        float hourAngle = -(300.0f * PI / 180.0f);
        mat4 hourT = translate(mat4(1.0f), vec3(0.0f, dialCenterY, 0.0f));
        hourT = rotate(hourT, hourAngle, vec3(0, 0, 1));
        hourT = translate(hourT, vec3(0.0f, -dialCenterY, 0.0f));
        HandHour->transformation(hourT); HandHour->draw(ourShader);

        Hub->transformation(I); Hub->draw(ourShader);

        // pendulum swing
        float swing = sinf(t * 1.3f) * 0.35f;
        vec3 pivot = vec3(0.0f, dialCenterY - 0.35f, 0.0f);
        mat4 P = translate(mat4(1.0f), pivot);
        P = rotate(P, swing, vec3(0, 0, 1));
        P = translate(P, -pivot);
        PendulumShaft->transformation(P); PendulumShaft->draw(ourShader);
        PendulumBob->transformation(P);   PendulumBob->draw(ourShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup
    auto delList = [&](std::vector<Polygon*>& v) { for (auto p : v) delete p; v.clear(); };
    delList(backPrism); delList(casePrism); delList(topShelfPrism); delList(bottomLedgePrism);
    delete BezelBack; delete BezelFront; delete BrassRing; delete DialFace; delete Hub;
    for (auto p : Marks) delete p;
    delete HandHour; delete HandMin; delete HandSec;
    delete PendulumShaft; delete PendulumBob;

    glfwTerminate();
    return 0;
}
