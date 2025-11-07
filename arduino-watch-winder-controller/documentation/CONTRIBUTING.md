# Contributing to Arduino Watch Winder Controller

Thank you for your interest in contributing! This project welcomes contributions from the community.

## How to Contribute

### Reporting Issues

If you encounter a problem:

1. **Search existing issues** to see if it's already reported
2. **Create a new issue** with:
   - Clear description of the problem
   - Steps to reproduce
   - Expected vs actual behavior
   - Hardware configuration (Arduino model, motor specs, etc.)
   - Photos if applicable

### Suggesting Enhancements

Have an idea for improvement?

1. **Open an issue** labeled "enhancement"
2. Describe the feature and use case
3. Explain why it would be useful

### Submitting Changes

1. **Fork the repository**
2. **Create a branch** for your feature:
   ```bash
   git checkout -b feature/your-feature-name
   ```
3. **Make your changes**
4. **Test thoroughly** with actual hardware
5. **Commit with clear messages**:
   ```bash
   git commit -m "Add feature: description of what you added"
   ```
6. **Push to your fork**:
   ```bash
   git push origin feature/your-feature-name
   ```
7. **Create a Pull Request** with:
   - Description of changes
   - Why the change is needed
   - How you tested it
   - Any relevant issue numbers

## Code Style Guidelines

- **Comments**: Add clear comments for complex logic
- **Variables**: Use descriptive names
- **Constants**: Use UPPER_CASE for constants
- **Functions**: Include description comments
- **Formatting**: Use consistent indentation (2 spaces)

## Testing Checklist

Before submitting, verify:

- [ ] Code compiles without errors or warnings
- [ ] Tested on actual hardware (Arduino + motors)
- [ ] Both motors work correctly
- [ ] Switches function as expected
- [ ] Timing is accurate
- [ ] No memory leaks or excessive power draw
- [ ] Documentation updated if needed

## Areas for Contribution

We'd especially welcome contributions in:

- **Additional motor driver support** (L298N, TB6612, etc.)
- **LCD/OLED display integration** for status
- **WiFi/Bluetooth control** features
- **Power optimization** techniques
- **Alternative microcontroller support** (ESP32, etc.)
- **PCB design** for custom boards
- **3D printable enclosure** designs
- **Better documentation** and tutorials
- **Translation** of documentation

## Questions?

Feel free to open an issue labeled "question" if you need clarification before contributing.

## Code of Conduct

Be respectful and constructive. We're all here to learn and build cool things together!

---

By contributing, you agree that your contributions will be licensed under the MIT License.