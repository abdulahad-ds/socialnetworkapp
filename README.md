# Social Network App

A console-based C++ social networking application that simulates core features of platforms like Facebook. It supports users, pages, posts, comments, likes, and memories, and allows full interaction between these entities. All data is read from and written to files, ensuring persistence of user activity.

## Features

- **User System**
  - Register and store new users  
  - Manage friendships  
  - View all users, friends, and liked pages  

- **Pages**
  - Like/unlike pages  
  - View page posts and interact with them  

- **Posts**
  - Create posts with optional activities (feeling, thinking, etc.)  
  - Like and comment on posts  
  - Share posts as "memories"  
  - Posts sorted by date  

- **Comments**
  - Add comments on any post (limit: 10 per post)  

- **Likes**
  - Like posts and view who liked them  

- **Memories**
  - Share and view posts from the past as memories  
  - Automatically show memories from current and previous years  

- **File-Based Persistence**
  - Supports loading and saving of:
    - Users (`user.txt`)  
    - Pages (`page.txt`)  
    - Posts (`post.txt`)  
    - Comments (`comment.txt`)  
    - Memories (`memories.txt`)  
  - Automatically updates all files when changes are made in the app  

## Project Structure

- mainfile.cpp # Contains complete logic of the application
- user.txt # Stores user data (name, ID, friends, liked pages)
- page.txt # Stores page data (name, ID, liked by users)
- post.txt # Stores all posts with meta info
- comment.txt # Stores comments with timestamps and authorship
- memories.txt # Stores shared memories

## How to Run

1. Open the project in Visual Studio or any C++ IDE.  
2. Ensure the `.txt` data files are in the same directory as the executable.  
3. Build and run the project.  
4. Interact via the command-line interface.  

## Concepts Used

- Object-Oriented Programming (Inheritance, Aggregation, Composition)  
- File Handling (Read, Write, Append)  
- Dynamic Memory Management  
- Console-based UI Design  
- Custom Data Structures  

## Dependencies

- C++17 or later  
- Standard Library only (no external libraries used)  

## Future Improvements

- GUI integration (e.g., Qt)  
- Admin panel for moderation  
- Post/media attachments  
- Real-time clock integration  