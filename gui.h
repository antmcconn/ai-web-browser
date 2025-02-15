#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[50];
    int x, y;
    int width, height;
    char label[100];
} Button;

typedef struct {
    char text[200];
    int x, y;
    int width, height;
} Label;

typedef struct {
    char title[100];
    int width, height;
    Button *buttons[10];
    Label *labels[10];
    int numButtons;
    int numLabels;
} Window;

typedef enum {
    EVENT_BUTTON_CLICKED,
    EVENT_MOUSE_MOVE,
    EVENT_WINDOW_CLOSE
} EventType;

void handleEvent(EventType event) {
    switch (event) {
        case EVENT_BUTTON_CLICKED:
            printf("Button clicked!\n");
            break;
        case EVENT_MOUSE_MOVE:
            printf("Mouse moved.\n");
            break;
        case EVENT_WINDOW_CLOSE:
            printf("Window close event triggered.\n");
            break;
    }
}

Button* createButton(const char *label, int x, int y, int width, int height) {
    Button *button = (Button *)malloc(sizeof(Button));
    strcpy(button->label, label);
    button->x = x;
    button->y = y;
    button->width = width;
    button->height = height;
    strcpy(button->name, "Button");
    return button;
}

Label* createLabel(const char *text, int x, int y, int width, int height) {
    Label *label = (Label *)malloc(sizeof(Label));
    strcpy(label->text, text);
    label->x = x;
    label->y = y;
    label->width = width;
    label->height = height;
    return label;
}

Window* createWindow(const char *title, int width, int height) {
    Window *window = (Window *)malloc(sizeof(Window));
    strcpy(window->title, title);
    window->width = width;
    window->height = height;
    window->numButtons = 0;
    window->numLabels = 0;
    return window;
}

void addButtonToWindow(Window *window, Button *button) {
    if (window->numButtons < 10) {
        window->buttons[window->numButtons] = button;
        window->numButtons++;
        printf("Added button '%s' to the window at position (%d, %d)\n", button->label, button->x, button->y);
    } else {
        printf("Max button limit reached!\n");
    }
}

void addLabelToWindow(Window *window, Label *label) {
    if (window->numLabels < 10) {
        window->labels[window->numLabels] = label;
        window->numLabels++;
        printf("Added label '%s' to the window at position (%d, %d)\n", label->text, label->x, label->y);
    } else {
        printf("Max label limit reached!\n");
    }
}

void runEventLoop(Window *window) {
    printf("Running event loop for window '%s'...\n", window->title);
    handleEvent(EVENT_BUTTON_CLICKED);
    handleEvent(EVENT_MOUSE_MOVE);
    handleEvent(EVENT_WINDOW_CLOSE);
}
