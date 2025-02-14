#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <ncurses.h>
#include <ctype.h>

// Function to handle the response data
size_t write_callback(void *ptr, size_t size, size_t nmemb, char *data) {
    strcat(data, ptr);
    return size * nmemb;
}

// Simple HTML parsing: Extracts <a> links and <h1>, <h2>, <p> tags
void parse_html(char *data) {
    char *ptr = data;
    int in_tag = 0;
    int in_link = 0;

    while (*ptr) {
        if (*ptr == '<') {
            in_tag = 1;
        } else if (*ptr == '>') {
            in_tag = 0;
            if (in_link) {
                // End of <a> tag
                in_link = 0;
            }
        } else if (in_tag) {
            // Skip the content inside tags
            if (strncmp(ptr, "<a href=", 8) == 0) {
                in_link = 1;
                printw("\n[Link] ");
            }
        } else if (*ptr == '\n') {
            // Skip newlines
            continue;
        } else if (isspace(*ptr)) {
            // Ignore extra spaces
            continue;
        } else {
            printw("%c", *ptr); // Print visible characters
        }
        ptr++;
    }
}

// Function to fetch the webpage using libcurl
void fetch_webpage(const char *url, char *data) {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

int main() {
    char url[256];
    char data[10000] = ""; // Store webpage content here
    char history[10][256];  // History of URLs for back navigation
    int history_index = 0;  // Track current history position

    // Initialize ncurses
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    while (1) {
        // Display URL input prompt
        clear();
        printw("Enter URL (or 'back' to go back, 'exit' to quit): ");
        refresh();
        echo();
        getstr(url); // Get URL input from user

        if (strcmp(url, "exit") == 0) {
            break; // Exit the program
        }

        if (strcmp(url, "back") == 0 && history_index > 0) {
            history_index--; // Go back to the previous URL
            strcpy(url, history[history_index]);
        }

        // Fetch the webpage
        fetch_webpage(url, data);

        // Add the current URL to history
        if (history_index < 10) {
            strcpy(history[history_index], url);
            history_index++;
        } else {
            // Shift history to make space for the new URL
            for (int i = 1; i < 10; i++) {
                strcpy(history[i - 1], history[i]);
            }
            strcpy(history[9], url);
        }

        // Parse and display the content
        clear();
        printw("Webpage Content:\n\n");
        parse_html(data); // Simplified HTML parsing
        printw("\n\nPress any key to continue...");
        refresh();
        getch();
    }

    // End ncurses
    endwin();

    return 0;
}
