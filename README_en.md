# Character Alphabet Recognition and Display on a Flip-dot 7-Segment Display

## Introduction

The assignment was to utilize an interesting type of electromechanical flip-dot display and demonstrate its usage. For the project, I had access to a display with dimensions of 4 rows, each with 7 characters. The basic assignment was to display simple messages and measured temperature and humidity. After completing this task, I created a neural network that recognizes letters of the character alphabet. This data is then displayed on the display.

## Project Solution

How the project works can be seen in this video:
[![video](https://img.youtube.com/vi/IdzLrns7vJk/0.jpg)](https://www.youtube.com/watch?v=IdzLrns7vJk)

## Equipment

### Flip-dot 7-segment Display

Communication with the display is possible using an RS-485 converter at a speed of 57600 baud. Characters on the display cannot be set individually but all at once using a data frame specified by the manufacturer[^1]:
