#include <opencv2/opencv.hpp>
#include <iostream>


const char grad[] = {' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'}; // Массив символов

cv::Mat createAsciiArt(const cv::Mat& image, int new_width = 100) {
    // Масштабируем изображение до нового размера
    int new_height = new_width * image.rows / image.cols;
    cv::Mat resized_image;
    cv::resize(image, resized_image, cv::Size(new_width, new_height), 0, 0, cv::INTER_LINEAR);

    // Создаем пустое изображение для ASCII-арта
    cv::Mat ascii_art(new_height * 10, new_width * 10, CV_8UC3, cv::Scalar(0, 0, 0));

    // Проходим по каждому пикселю изображения
    for (int y = 0; y < resized_image.rows; y++) {
        for (int x = 0; x < resized_image.cols; x++) {
            // Получаем интенсивность пикселя
            uchar intensity = resized_image.at<uchar>(y, x);

            // Выбираем символ из массива grad
            char asciiChar = grad[intensity * (sizeof(grad) - 1) / 255];

            // Рисуем символ на изображении
            cv::putText(ascii_art, std::string(1, asciiChar), cv::Point(x * 10, y * 10 + 10),
                        cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 255, 255), 1);

/*
x и y — это координаты текущего пикселя в уменьшенном изображении.
x * 10 и y * 10 — это масштабирование координат, чтобы символы не накладывались друг на друга.
Каждый символ занимает примерно 10x10 пикселей на изображении ascii_art.
+ 10 по оси Y используется для выравнивания символов, так как cv::putText рисует текст относительно нижнего левого у
*/
        }
    }

    return ascii_art;
}


int main(){

    cv::VideoCapture cap ("BadApple.mp4");


    if (!cap.isOpened()) {
        std::cerr << "Ошибка: Не удалось открыть видеофайл!" << std::endl;
        return -1;
    }

    cv::Mat frame;
    cv::Mat grayFrame;


    while(true)
    {
        cap >> frame;

        cv::cvtColor(frame, grayFrame, cv::COLOR_RGB2GRAY);

        cv::Mat asciiImage = createAsciiArt(grayFrame, 100);

        cv::imshow("Window", asciiImage);
        //cv::imshow("Origin", frame);

        cv::waitKey(30);
    }


    cap.release();
    cv::destroyAllWindows();

    return 0;
}