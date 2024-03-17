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
    
    // �ʱ� ��ǥ
    float x[10] = {-0.2f, -0.5f, 0.2f, 0.5f, 0.2f, 0.5f, -0.2f, -0.5f, -0.2f, -0.5f};
    float y[10] = {-0.2f, -0.5f, -0.2f, -0.5f, 0.2f, 0.5f, 0.2f, 0.5f, -0.2f, -0.5f};
    
    bool isrotating = false; // ȸ�� ����
    double velo = 1.0; // ���ӵ�
    double starttime = 0; // ���� ȸ���� ���� �ð�
    double stoptime = 0; // ������ ȸ���� ���� �ð�
    double current = 0; // ���α׷� ���� �� �������� ����(���ӵ� ��� X)
    double realtime = 0; // velocity�� ���� �ǽð����� ���ӵ��� ���ϴ� ���� << ��꿡 ���

    double lastpressed = 0; // Ű�� ���������� ���� �ð�: Ű �ݺ��Է� ������
    double debouncetime = 0.2; // Ű �ݺ��Է� ������ �ð�

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

        // realtime (���� ��) �� ����ؾ� �ǽð����� �ӵ��� �ø��� ���� �� �ִ�.
        // = ���� loop�� ���� �� + (����ð� - ���� loop�� �ð�) * ���� ���ӵ�
        // �� loop���� �޶����� ���ӵ��� ��ȭ�� loop ������ �ð� ���� ���� ���� ���� ����Ѵ�.
        realtime = realtime + velo*(glfwGetTime() - current); // �ǽð� �� ���
        current = glfwGetTime(); // ���� loop ��꿡�� ����� current ����
        
        // Ű �Է¿� ���� ȸ�� �ӵ� ����
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

        // ���� �� �����̽��� ��� ó��
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && current - lastpressed > debouncetime) {
            lastpressed = current;

            if (isrotating) { // ȸ�� ���̰� ������ ���̸�
                // �����ϱ� �� ������ �ð� t0 ��� = ������ ������ �ð� + (���� �ð� - ������ ȸ�� ������ �ð�)
                // ���ο� stoptime�� �����ϱ� �� ����Ѵ�.
                stoptime = realtime - starttime + stoptime; // ���� ���� ���� = ���ӵ��� ���ϴ� ��(realtime)���� �پ�� �Ѵ�.
                // stoptime = a + wt = �����ð� + ���ӵ�*(����ð� - ȸ�����۽ð�) (�̹� ���ӵ��� ������)
			}
            else { // �������̰� ȸ���� ���̸�
                starttime = realtime; // ���� �ð��� ������Ʈ �Ͽ� �ǽð� rotation�� ����Ѵ�.
			}
            isrotating = !isrotating; // ȸ�� ���� ���

        }
        // R Ű ��� ó�� : �ʱ�ȭ �� ����
        else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && current - lastpressed > debouncetime) {
            lastpressed = current; // ������ ���� �ð� ������Ʈ
            isrotating = false; // ȸ�� ����
            stoptime = 0; // ���� �� ���� �ʱ�ȭ
        }

        // isrotating ���¿� ���� �ǽð����� ��ǥ ���� ����
        if (isrotating) { // ȸ�� �� ���¸�
            double rotatingtime = realtime - starttime + stoptime; // �ǽð� ���� ���
            glColor3f(1.f, 0.7f, 0.f);
            glBegin(GL_TRIANGLE_STRIP);
            for (int i = 0; i < 10; i++) {
                float cx = x[i] * cos(rotatingtime) - y[i] * sin(rotatingtime);
                float cy = y[i] * cos(rotatingtime) + x[i] * sin(rotatingtime);
                glVertex3f(cx, cy, 0.f);
            }
            glEnd();
		}
        else { // ���� �� ���¸� ���� Ű �Է� ��� ����� ������ ������ ���
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
