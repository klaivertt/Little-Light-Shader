# ✨ **Light Shader Project**

This project demonstrates a simple light shader effect implemented using **CSFML** (C binding for SFML). The light dynamically follows the mouse cursor and adjusts its position and intensity based on distance, creating a glowing effect.

## 🚀 **Features**
- Real-time lighting effect that follows the mouse cursor.
- Adjustable light color, radius, and intensity.
- OpenGL-compatible shader written in GLSL.
- Smooth Y-axis correction for consistent rendering across systems.

## 🖥️ **How It Works**

### 📂 **Project Structure**
- **`main.c`**: The main program handling the window, events, and shader rendering.
- **`shader.frag`**: The fragment shader code that calculates the light effect (located in `x64/Debug/`).

### 🔧 **Shader Logic**
1. **Mouse Input**: Captures the mouse's position and sends it to the shader.
2. **Y-Axis Correction**: Adjusts the mouse position to match OpenGL's coordinate system.
3. **Light Calculation**: Computes light intensity based on the pixel's distance from the light source.
4. **Smooth Light Falloff**: Uses the `smoothstep` function for a soft transition around the light radius.

## 🛠️ **Getting Started**

### 📦 **Setup Instructions**
1. Clone this repository:
   ```bash
   git clone https://github.com/username/light-shader-project.git
   cd light-shader-project
   ```
2. Open the project with **Visual Studio** and build it.

3. Run the compiled executable to see the shader in action.

## 🎨 **Customization**
You can modify the following shader parameters in `main.c`:
- **`radius`**: Controls the size of the light.
- **`lightColor`**: Adjusts the light's RGB color.
- **`screenHeight`**: Matches the height of the window to correct Y-axis inversion.

Example:
```c
sfShader_setFloatUniform(shader, "radius", 200.0f);
sfShader_setVec3Uniform(shader, "lightColor", (sfGlslVec3) { 0.7, 0.8, 1.0 }); // Soft blue
```

## 📚 **References**
- **SFML Documentation**: [https://www.sfml-dev.org/documentation/](https://www.sfml-dev.org/documentation/)
- **OpenGL Shading Language**: [GLSL Reference](https://www.khronos.org/opengl/wiki/Core_Language_(GLSL))

## 🤝 **Contributing**
Contributions are welcome! Feel free to open issues or submit pull requests.

## 📜 **License**
This project is licensed under the **MIT License**.
