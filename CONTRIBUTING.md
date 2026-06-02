## Contribution Guidelines

We welcome contributions to improve the Archiver Utility! If you'd like to contribute, please follow these steps:

1. **Fork the Repository:** Create your own fork of the project.
2. **Create a Feature Branch:** Branch off from `main` for your work.
   ```bash
   git checkout -b feature/your-feature-name
   ```
3. **Adhere to Code Standards:**
   * **Memory Management:** Ensure all `malloc` and `realloc` calls are paired with appropriate `free()` operations. We highly recommend testing your changes with `valgrind` to prevent memory leaks.
   * **Binary Safety:** When altering `struct` definitions in `header.h`, be mindful of struct padding and binary compatibility.
   * **Style:** Maintain the existing C formatting conventions.
4. **Commit Your Changes:** Write clear, concise commit messages detailing the "why" and "what" of your changes.
5. **Push to Your Fork:**
   ```bash
   git push origin feature/your-feature-name
   ```
6. **Submit a Pull Request:** Open a PR against the `main` branch of the original repository. Include a detailed description of your changes and any relevant testing performed.
