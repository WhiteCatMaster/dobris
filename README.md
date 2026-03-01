# dobris

dobris is an open-source desktop application built with Electron, providing a centralized interface for launching and managing classic id Tech 1 engine games. It integrates with existing game engine ports like Chocolate Doom and offers configuration support for titles such as Hexen and Strife. The project also features C++ components for core logic and includes dedicated management for game-related music assets.

## Features

*   *Desktop Application*: Built using Electron for a cross-platform user interface, ensuring broad compatibility.
*   *Game Launcher*: Provides a unified interface for discovering, launching, and managing classic game titles.
*   *Chocolate Doom Integration*: Seamlessly works with the Chocolate Doom port of the original Doom engine.
*   *Hexen & Strife Support*: Includes robust configuration and launch capabilities specifically for Hexen and Strife.
*   *Music Management*: Features dedicated functionality for organizing and utilizing game music resources, enhancing the classic gaming experience.
*   *C++ Core Components*: Utilizes C++ for robust backend functionalities and low-level game engine interactions, ensuring performance and precise control.
*   *Continuous Integration*: Ensures code quality, consistency, and maintainability through automated workflows for C++ linting and checking.
*   *Containerization Ready*: Supports development and deployment within Docker environments, facilitating consistent build and runtime conditions.
*   *Contribution Guidelines*: Clear CONTRIBUTING.md and ISSUE_TEMPLATE.md are provided to guide community involvement.

## Tech Stack

dobris is developed using a combination of modern web technologies for its user interface and robust native languages for its core functionalities.

*   *Languages*: C++, HTML, CSS, JavaScript
*   *Application Framework*: Electron
*   *Build Systems*: CMake, Autotools (for C++ components)
*   *Package Managers*: npm, Yarn

## Installation Guide

To set up dobris for development or local usage, follow these steps.

### Prerequisites

Ensure you have the following installed on your system:

*   *Node.js*: The latest LTS version is recommended (includes npm).
*   *Yarn* (Optional, if preferred over npm): Install globally via npm install -g yarn.
*   *C++ Development Environment*:
    *   A C++ compiler (e.g., GCC, Clang, MSVC).
    *   CMake for building C++ components.
    *   Autotools (autoconf, automake, libtool) for certain build scripts.

### Steps

1.  *Clone the Repository*:
    bash
    git clone https://github.com/dobris-org/dobris.git
    cd dobris
    
    (Note: Replace https://github.com/dobris-org/dobris.git with the actual repository URL)

2.  *Install Node.js Dependencies*:
    Using npm:
    bash
    npm install
    
    Or using Yarn:
    bash
    yarn install
    

3.  *Run the Application*:
    To start the application in development mode:
    Using npm:
    bash
    npm start
    
    Or using Yarn:
    bash
    yarn start
    

4.  *Build for Production (Optional)*:
    To build the application for distribution:
    Using npm:
    bash
    npm run dist
    
    Or using Yarn:
    bash
    yarn dist
    
    This command will package the application for your operating system, typically placing the output in a dist/ directory.

## Usage Instructions

Upon launching dobris, you will be presented with an intuitive interface designed to manage your classic game library.

1.  *Game Discovery*: The application will guide you through locating your installed game files and WADs for titles such as Doom, Hexen, and Strife.
2.  *Configuration*: Navigate through the application to set up game paths, modify specific engine parameters for supported games, and customize launch options.
3.  *Launch Games*: Easily launch your configured games directly from the dobris interface.
4.  *Music Management*: Utilize the dedicated section for organizing and previewing game music files, which can then be associated with specific game launches.

Refer to the in-application interface for detailed instructions on specific features and settings.

## Contributing

We warmly welcome contributions from the community to enhance dobris. Whether you're reporting a bug, suggesting a new feature, or submitting code, your input is valuable.

*   Please refer to our comprehensive [CONTRIBUTING.md](CONTRIBUTING.md) guide for detailed information on how to get involved, set up your development environment, and adhere to our contribution workflow.
*   When reporting issues, please use the provided [ISSUE_TEMPLATE.md](.github/ISSUE_TEMPLATE.md) to ensure all necessary information is included.
*   All C++ code contributions are subject to automated linting and checking via our Continuous Integration pipelines to maintain high code quality and consistency.
