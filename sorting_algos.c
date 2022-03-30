#include <stdio.h>
#include <raylib.h>

void printArray(int arr[], int size);

void bubbleSort(int *arr, int n, Rectangle *rectangles);

const int screenWidth = 1200;
const int screenHeight = 600;
static Rectangle textboxBounds[1] = {
    { 10, 10, 160, 24 }, 
};

static Texture2D checked = { 0 };

#include <string.h>

bool ButtonXY(int x, int y, char const *text) {
    int padding = 2;
    int swidth = strlen(text) * 10;
    int bodywidth = swidth + padding * 2;
    int textPx = 16;
    int height = textPx + 2 * padding;
    Rectangle body = {x, y, bodywidth, height};
    Rectangle textbody = {body.x + padding, body.y + padding, body.width - 4, body.height - 4};
    
    // Update control
    Vector2 mouse = GetMousePosition();
    bool pressed = (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, body));

    // Draw control
    DrawTexture(checked, body.x, body.y, WHITE);
    DrawRectangleRec(body, BLACK);
    DrawRectangleLinesEx(body, 1, GRAY);
    DrawText(text, textbody.x, textbody.y, textbody.height, WHITE);
    
    return pressed;
}

// Draw a color button, if clicked return true
bool ColorButton(Rectangle bounds, Color color, char *text)
{
    return ButtonXY(bounds.x, bounds.y, text);
    Rectangle body = {bounds.x + 2, bounds.y + 2, bounds.width - 4, bounds.height - 4};
    Rectangle textbody = {bounds.x + 4, bounds.y + 4, bounds.width - 8, bounds.height - 8};
    
    // Update control
    Vector2 mouse = GetMousePosition();
    bool pressed = (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, bounds));

    // Draw control
    DrawTexture(checked, body.x, body.y, WHITE);
    DrawRectangleRec(body, BLACK);
    DrawRectangleLinesEx(bounds, 1, BLACK);
    DrawText(text, textbody.x, textbody.y, 16, WHITE);
    
    return pressed;
}

void quickSort(int *arr, int n, Rectangle *rects);
void quickSortHoore(int *arr, int n, Rectangle *rects);
void radixsort(int arr[], int n);

void rebuildRects(Rectangle *rects, int *dataset, int count) {
    for (int i = 0; i < count; ++i)
    {
        Rectangle r = {i * GetScreenWidth() / count,
                       GetScreenHeight() - dataset[i],
                       GetScreenWidth() / count,
                       dataset[i]};
        rects[i] = r;
    }
}

int main(void)
{
    InitWindow(screenWidth, screenHeight, "raylib [shapes] example - sorting algorithm: bubblesort");


    const int count = 500;
    int dataset[count];
    int datasetbefore[count];

    for (int i = 0; i < count; i++) {
        datasetbefore[i] = (screenHeight * (i + 1)) / count;
    }
    memcpy(dataset, datasetbefore, sizeof(*dataset) * count);

    // for (int i = 0; i < count; ++i) {
    //     int dataamt = count - i - 1;
    //     int dataidx = GetRandomValue(0, dataamt);
    //     dataset[i] = datasetbefore[dataidx];
    //     datasetbefore[dataidx] = datasetbefore[dataamt];
    // }

    Rectangle rects[count];
    rebuildRects(rects, dataset, count);

    BeginDrawing();

    ClearBackground(WHITE);
    for (int k = 0; k < count; k++) {
        DrawRectangleRec(rects[k], BLACK);
    }
    EndDrawing();

    BeginDrawing();

    ClearBackground(WHITE);
    for (int k = 0; k < count; k++) {
        DrawRectangleRec(rects[k], BLACK);
    }
    EndDrawing();

    SetTargetFPS(144);

#define ARR_L(arr) (sizeof (arr) / sizeof(*(arr)))
    bool clicked = false;
    char *arr[] = {
        "Bubblesort",
        "Quicksort",
        "Hoore QuickSort",
        "Radix",
    };
    int clickednum = 0;
    while (!clicked) {
        BeginDrawing();
        for (size_t i = 0; i < ARR_L(arr); i++) {
            Rectangle r = textboxBounds[0];
            r.y += i * 30;
            bool c = ButtonXY(r.x, r.y, arr[i]);
            if (c) {
                clickednum = i;
                clicked = true;
            }
        }
        
        EndDrawing();
    }
    SetTargetFPS(144);
    if (clickednum == 0) {
        bubbleSort(dataset, count, rects);
    } else if (clickednum == 1) {
        quickSort(dataset, count, rects);
    } else if (clickednum == 2) {
        quickSortHoore(dataset, count, rects);
    } else {
        radixsort(dataset, count);
    }

    rebuildRects(rects, dataset, count);
    
    BeginDrawing();

    ClearBackground(WHITE);
    for (int i = 0; i < count; i++) {
        DrawRectangleRec(rects[i], GREEN);
    }
    DrawFPS(0, 0);
    EndDrawing();
    BeginDrawing();

    ClearBackground(WHITE);
    for (int i = 0; i < count; i++) {
        DrawRectangleRec(rects[i], GREEN);
    }
    DrawFPS(0, 0);
    EndDrawing();
    
    // printf("Sorted array: \n");
    // printArray(dataset, count);

    SetTargetFPS(24);
    for (;;) {
        BeginDrawing();
        EndDrawing();
    }
}

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void swapFloat(float *xp, float *yp)
{
    float temp = *xp;
    *xp = *yp;
    *yp = temp;
}

int getMax(int arr[], int n)
{
    int mx = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] > mx)
            mx = arr[i];
    return mx;
}

void drawRadix(int *arr, int n) {
    Rectangle res[n];
    rebuildRects(res, arr, n);
    BeginDrawing();
    ClearBackground(WHITE);
    for (size_t i = 0; i < n; i++) {
        DrawRectangleRec(res[i], BLACK);
    }
    DrawFPS(0, 0);
    EndDrawing();
}

void countSort(int arr[], int n, int exp)
{
    int output[n]; // output array
    int count[10] = { 0 };
 
    // Store count of occurrences in count[]
    for (int i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;
 
    // Change count[i] so that count[i] now contains actual
    //  position of this digit in output[]
    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];
 
    // Build the output array
    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
        drawRadix(output, n);
    }
 
    // Copy the output array to arr[], so that arr[] now
    // contains sorted numbers according to current digit
    memcpy(arr, output, sizeof(*arr) * n);
}
 
#include <math.h>

void radixRandomize(int arr[], int n) {
    // Find the maximum number to know number of digits
    int m = getMax(arr, n);
 
    // Do counting sort for every digit. Note that instead
    // of passing digit number, exp is passed. exp is 10^i
    // where i is current digit number
    int numk = log10(m);
    for (int i = 0; i < numk; i++) {
        countSort(arr, n, pow(10, numk-1-i));
    }
}

// The main function to that sorts arr[] of size n using
// Radix Sort
void radixsort(int arr[], int n)
{
    // Find the maximum number to know number of digits
    int m = getMax(arr, n);
 
    // Do counting sort for every digit. Note that instead
    // of passing digit number, exp is passed. exp is 10^i
    // where i is current digit number
    for (int exp = 1; m / exp > 0; exp *= 10)
        countSort(arr, n, exp);
}

void printArray(int *arr, int size)
{
    char *start_str[] = {"\"", ", "};
    for (int i = 0; i < size; i++) {
        printf("%s%d", start_str[i != 0], arr[i]);
    }

    printf("\"\n");
}

void swapNums(int *arr, int i, int j, Rectangle *rects) {
    swap(&arr[j], &arr[i]);

    swapFloat(&rects[j].height, &rects[i].height);
    swapFloat(&rects[j].y, &rects[i].y);
}

void drawQuickSorted(int *arr, int n, Rectangle *rects, int pivot, int sorted, int selected) {
    BeginDrawing();
    ClearBackground(WHITE);
    for (size_t i = 0; i < n; i++) {
        if (i < sorted) {
            DrawRectangleRec(rects[i], GREEN);
        } else if (i == pivot) {
            DrawRectangleRec(rects[i], PURPLE);
        } else if (i == selected) {
            DrawRectangleRec(rects[i], RED);
        } else if (i <= pivot) {
            DrawRectangleRec(rects[i], BLACK);
        } else {
            DrawRectangleRec(rects[i], BLACK);
        }
    }
    DrawFPS(0, 0);
    EndDrawing();
}

int partitionHoore(int *arr, int n, int lo, int hi, Rectangle *rects) {
    int pividx = (hi + lo) / 2;
    int pivot = arr[pividx];
    int i = lo - 1;
    int j = hi + 1;
    drawQuickSorted(arr, n, rects, pividx, lo, i);
    for (;;) {
        do {
            i++;
        } while (arr[i] < pivot);
        do {
            j--;
        } while (arr[j] > pivot);
        if (i >= j) {
            return j;
        }
        drawQuickSorted(arr, n, rects, pividx, lo, i);
        swapNums(arr, i, j, rects);
        drawQuickSorted(arr, n, rects, pividx, lo, i);
    }
}

int quickSortHoores(int *arr, int n, int lo, int hi, Rectangle *rects) {
    if (lo >= 0 && hi >= 0 && lo < hi) {
        int p = partitionHoore(arr, n, lo, hi, rects);
        drawQuickSorted(arr, n, rects, p, lo, 0);
        quickSortHoores(arr, n, lo, p, rects);
        quickSortHoores(arr, n, p + 1, hi, rects);
        drawQuickSorted(arr, n, rects, p, hi, 0);
    }
}

void quickSortHoore(int *arr, int n, Rectangle *rects) {
    quickSortHoores(arr, n, 0, n-1, rects);
}

int partitionLomuto(int *arr, int n, int lo, int hi, Rectangle *rects) {
    int pividx = hi;
    int pivot = arr[pividx];
    int i = lo - 1;
    drawQuickSorted(arr, n, rects, pividx, lo, i);
    for (size_t j = lo; j < hi; j++) {
        if (arr[j] <= pivot) {
            i++;
            drawQuickSorted(arr, n, rects, pividx, lo, i);
            swapNums(arr, i, j, rects);
            drawQuickSorted(arr, n, rects, pividx, lo, i);
        }
    }
    i++;
    drawQuickSorted(arr, n, rects, pividx, lo, i);
    swapNums(arr, i, pividx, rects);
    drawQuickSorted(arr, n, rects, pividx, lo, i);
    return i;
}

void quickSorts(int *arr, int n, int lo, int hi, Rectangle *rects) {
    if (lo >= hi || lo < 0) return; 
    drawQuickSorted(arr, n, rects, 0, lo, 0);
    int pivot = partitionLomuto(arr, n, lo, hi, rects);
    quickSorts(arr, n, lo, pivot - 1, rects);
    quickSorts(arr, n, pivot + 1, hi, rects);
    drawQuickSorted(arr, n, rects, pivot, hi, 0);
}

void quickSort(int *arr, int n, Rectangle *rects) {
    quickSorts(arr, n, 0, n - 1, rects);
}

void drawSorted(int *arr, int n, Rectangle *rects, int current, int sorted_s, int sorted_e) {
    BeginDrawing();

    ClearBackground(WHITE);

    for (int k = 0; k < n; k++) {
        // we are 100% sure that all numbers on the right side of n - i - 1 is fully sorted 
        if (k == current) {
            // the rectangle that we are currently looking at
            DrawRectangleRec(rects[k], RED);
        } else if (sorted_s <= k && k <= sorted_e) {
            DrawRectangleRec(rects[k], GREEN);
        } else {
            DrawRectangleRec(rects[k], BLACK);
        }
    }

    DrawFPS(0, 0);
    EndDrawing();
}

void bubbleSort(int *arr, int n, Rectangle *rectangles)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                swapNums(arr, j, j+1, rectangles);
                drawSorted(arr, n, rectangles, j + 1, n - i, n - 1);
                /*
                BeginDrawing();

                ClearBackground(WHITE);

                for (int k = 0; k < n; k++) {
                    // we are 100% sure that all numbers on the right side of n - i - 1 is fully sorted 
                    if (n - i - 1 < k) {
                        DrawRectangleRec(rectangles[k], GREEN);
                    } else {
                        DrawRectangleRec(rectangles[k], BLACK);
                    }
                }
                if (j + 1 < n - i - 1) {
                    // the rectangle that we are currently looking at
                    DrawRectangleRec(rectangles[j + 1], RED);
                }

                DrawFPS(0, 0);
                EndDrawing();*/
            }
        }
    }
}