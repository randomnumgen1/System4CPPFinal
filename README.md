> [!CAUTION]
> Some classes may be deemed experimental until their usage is fully validated.
> Some things will be broken until v1.0 release.

# System4CPP
This cross-platform framework, compatible with Linux, Windows, ARM and x64 and perhaps Android, is designed to make developing applications and games easier. 
It uses coding styles that allow for effortless porting to other programming languages and ensure the code is beginner-friendly. 
Furthermore, the framework's types are organized so that users can easily understand the structure or class size just by looking at them. 
Additionally, the code is binary compatible with plugin architectures, guaranteeing that both the plugin and the application consistently interpret the data type layout being shared between them.

# Status

https://github.com/randomnumgen1/System4CPPFinal/wiki/Project-Status


# Features

+ **Cross-Platform Compatibility**: Works on Linux, Windows, ARM and x64.
+ **Simplifies Development**: Designed to make developing applications and games easier.
+ **Coding Styles for Portability**: Uses coding styles that allow for effortless porting to other programming languages.
+ **Beginner-Friendly Code**: Strikes a balance between simplicity for beginners and optimized for high performance.
+ **Intuitive Type Structure**: Structures are designed for raw memory copies, while classes encapsulate pointer-based relationships and complex behavior.
+ **Binary Compatibility with Plugin Architectures**: Ensures both the plugin and the application consistently interpret the data type layout being shared.
+ **Hashing Algorithms**: We aim to provide support for all officially recognized hashing algorithms, alongside those employed in reputable cryptocurrencies. These algorithms have been rigorously tested and verified for security, offering reliable options for data integrity verification, digital signatures, and more.
+ **Encryption and Public/Private Key Algorithms**: Our encryption capabilities aim to cover a diverse range of official encryption algorithms, as well as those widely adopted in the cryptocurrency space. This ensures secure data transmission and storage through robust public and private key cryptography, safeguarding sensitive information against unauthorized access.
+ **Compression Algorithms**: Our platform aims to support a full spectrum of compression algorithms, enabling efficient data storage without compromising the integrity or accessibility of the information. This feature enhances performance while minimizing resource usage, making it ideal for both local and cloud storage solutions.
+ **Checksum Algorithms**: Provide a fast, lightweight way to verify data integrity—ideal for validating network packets, though they lack the cryptographic strength needed for security.
+ **File and Directory helpers** Makes file and folder operations beginner-friendly without sacrificing control
+ **Text-Based formats** Provides classes to parse CSV, INI, and JSON
+ **TrueType font loading** Enables parsing of TrueType (.ttf) files for text rendering.
+ **Drawing Canvas** provides a HTML Canvas–style interface for 2D drawing.
+ **Image Formats (PNG, JPEG, BMP,...)** in progress support for loading various image formats.
+ **3D model Formats (OBJ, FBX, GLTF...)** in progress support for loading various 3d models.
+ **Game Math** Vector2, Vector3, Matrix4x4 etc...
+ **Controller Support (PS, Xbox, 8bitdo)** Planned cross-platform support for PS, Xbox, and 8bitdo controllers.
+ **Networking (TCP/UDP, HTTP, WebSocket)** Planned cross-platform support for TCP/UDP sockets, HTTP clients, and WebSocket.
+ **Audio** Planned cross-platform support for audio.
+ **Animation System** Planned cross-platform support for Animation System.





## Defines

### General

|            Define            |                Comment               |
| ---------------------------- | ------------------------------------ |
| `SYSTEM_PROFILER_ENABLED`    |                                      |
| `SYSTEM_EXPERIMENTAL_ENABLED`| EXPERIMENTAL code that may have bugs |
| `SYSTEM_SOCKETS_ENABLED`     | enable cross platform sockets        |



### Graphics 


|         Define         |   Comment     |
| ---------------------- | ------------- |
| SYSTEM_GRAPHICS_VULKAN |     NA        |
| SYSTEM_GRAPHICS_OPENGL |     NA        |

### Audio



|         Define         |   Comment                                |
| ---------------------- | ---------------------------------------- |
| SYSTEM_AUDIO_OPENAL    |     NA                                   |
| SYSTEM_AUDIO_FMOD      |     licensing fee may be required        |

### PHYSICS



|         Define         |   Comment                                |
| ---------------------- | ---------------------------------------- |
| SYSTEM_PHYSICS_BULLET  |     NA                                   |



**Gamepad not working?**

```bash
sudo usermod -a -G input $USER
```

dependancies for Tests


```
sudo apt-get update && sudo apt-get install -y libgtest-dev libglm-dev
```


# Credits

+ **BigInt**
Author: Syed Faheel Ahmad (faheel@live.in)


+ **glgen**
Author: MetricPanda