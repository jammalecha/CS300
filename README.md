# CS300

1. What was the problem you were solving in the projects for this course?

   The goal of the projects in this course was to develop a course advising program that could store, organize, and retrieve course information and prerequisites. In Project One, I analyzed the run-time and memory requirements of different data structures to determine which would be most appropriate for the application. In Project Two, I implemented a C++ program using a hash table that loads course information form a CSV file, prints the courses in alphanumeric order, and allows users to search for individual courses and their prerequisites.

2. How did you approach the problem? Consider why data structures are important to understand.

   I approached the problem by considering how the course information neede3d to be stored and accessed. In Project One, I evaluated the advantages and disadvantages of different data structures before deciding to use a hash table for Project Two. Hash tables provide efficient lookups, making them useful for searching courses by their course numbers. Understanding data structures is important because choosing the appropriate structure can significantly affect a program's efficiency and complexity.

3. How did you overcome any roadblocks you encountered while going through the activities or project?

   One challenge I encountered was handling CSV files containing missing information, extra whitespace, or invalid course records. Initially, my program would reject the entire file when it encountered invalid data. I improved the validation logic to skip invalid records while preserving valid courses. I also added whitespace trimming and case-insensitive course searches to make the program more reliable and user-friendly. Testing different inputs helped me identify problems and verify that my changes worked correctly.
   Earlier on, I struggled with pointers but became quite confident in them, even sidetracked and looked in to usage of r and l values as some extracurricular work; although did not utilize it in these projects!

4. How has your work on this project expanded your approach to designing software and developing programs?

   These projects helped me understand the importance of planning how data will be organized before writing the program. I learned to break larger problems into smaller tasks and develop individual functions for loading data, validating prerequisites, searching for courses, and sorting course information. This approach made it easier to test each part of the program and identify issues without having to troubleshoot the entire application at once.

5. How has your work on this project evolved the way you write programs that are maintainable, readable, and adaptable?

   Working on these projects reinforced the importance of writing organized and readable code. I became more comfortable using descriptive variable names, comments, and functions with specific responsibilities. Implementing the hash table also helped me better understand dynamic memory management and the importance of properly releasing allocated memory. By separating the data structure from the functions responsible for loading and displaying information, I made the program easier to maintain and modify in the future.
