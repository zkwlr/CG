// HW2_Donut
// Immediate Mode Example using glfw

#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// window size
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    if (!glfwInit()) {
        std::cout << "Failed to initiallize GLFW" << std::endl;
        return -1;
    }; 

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HW2_Donut", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    // we don't have to use GLEW library in this program
    // cause immediate mode doesn't need any modern OpenGL features (core profile)
    
    // 초기 좌표
    float x[10] = {-0.2f, -0.5f, 0.2f, 0.5f, 0.2f, 0.5f, -0.2f, -0.5f, -0.2f, -0.5f};
    float y[10] = {-0.2f, -0.5f, -0.2f, -0.5f, 0.2f, 0.5f, 0.2f, 0.5f, -0.2f, -0.5f};
    
    bool isrotating = false; // 회전 여부
    double velo = 1.0; // 각속도
    double starttime = 0; // 현재 회전의 시작 시각
    double stoptime = 0; // 마지막 회전을 멈춘 시각
    double current = 0; // 프로그램 실행 후 절대적인 각도(각속도 계산 X)
    double realtime = 0; // velocity를 곱한 실시간으로 각속도가 변하는 각도 << 계산에 사용

    double lastpressed = 0; // 키를 마지막으로 누른 시각: 키 반복입력 방지용
    double debouncetime = 0.2; // 키 반복입력 방지용 시간

    // 
    // main loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        
        // setting the camera
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        float ratio = SCR_WIDTH / (float)SCR_HEIGHT;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        // orthographic projection of NDC
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // realtime (실제 각) 을 계산해야 실시간으로 속도를 늘리고 줄일 수 있다.
        // = 이전 loop의 실제 각 + (현재시각 - 이전 loop의 시각) * 현재 각속도
        // 매 loop마다 달라지는 각속도의 변화를 loop 사이의 시각 차에 곱해 실제 각을 계산한다.
        realtime = realtime + velo*(glfwGetTime() - current); // 실시간 각 계산
        current = glfwGetTime(); // 다음 loop 계산에서 사용할 current 갱신
        
        // 키 입력에 따른 회전 속도 조절
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && current - lastpressed > debouncetime) {
            lastpressed = current;
            if (velo > 0.2) {  // min velocity = 0.2
                velo = velo - 0.1;
                
			}

        }
        else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && current - lastpressed > debouncetime) {
            lastpressed = current;
            if (velo < 2.0) {  // max velocity = 2.0
                velo = velo + 0.1;
            }

        }

        // 루프 전 스페이스바 명령 처리
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && current - lastpressed > debouncetime) {
            lastpressed = current;

            if (isrotating) { // 회전 중이고 정지할 것이면
                // 정지하기 전 정지한 시간 t0 기록 = 마지막 정지한 시간 + (현재 시간 - 마지막 회전 시작한 시간)
                // 새로운 stoptime을 정지하기 전 기록한다.
                stoptime = realtime - starttime + stoptime; // 현재 각도 공식 = 각속도는 변하는 값(realtime)에만 붙어야 한다.
                // stoptime = a + wt = 정지시간 + 각속도*(현재시간 - 회전시작시간) (이미 각속도는 곱해짐)
			}
            else { // 정지중이고 회전할 것이면
                starttime = realtime; // 시작 시간을 업데이트 하여 실시간 rotation에 사용한다.
			}
            isrotating = !isrotating; // 회전 상태 토글

        }
        // R 키 명령 처리 : 초기화 후 정지
        else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && current - lastpressed > debouncetime) {
            lastpressed = current; // 마지막 누른 시간 업데이트
            isrotating = false; // 회전 정지
            stoptime = 0; // 정지 시 각도 초기화
        }

        // isrotating 상태에 따라 실시간으로 좌표 상태 설정
        if (isrotating) { // 회전 중 상태면
            double rotatingtime = realtime - starttime + stoptime; // 실시간 각도 계산
            glColor3f(1.f, 0.7f, 0.f);
            glBegin(GL_TRIANGLE_STRIP);
            for (int i = 0; i < 10; i++) {
                float cx = x[i] * cos(rotatingtime) - y[i] * sin(rotatingtime);
                float cy = y[i] * cos(rotatingtime) + x[i] * sin(rotatingtime);
                glVertex3f(cx, cy, 0.f);
            }
            glEnd();
		}
        else { // 정지 중 상태면 위의 키 입력 당시 기록한 정지한 각도로 대기
			glColor3f(1.f, 0.7f, 0.f);
			glBegin(GL_TRIANGLE_STRIP);
            for (int i = 0; i < 10; i++) {
                float cx = x[i] * cos(stoptime) - y[i] * sin(stoptime);
                float cy = y[i] * cos(stoptime) + x[i] * sin(stoptime);
                glVertex3f(cx, cy, 0.f);
			}
			glEnd();
		}   
        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}
