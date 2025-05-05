# Normalized Device Coordinates (NDC)

Normalized Device Coordinates is a small 3D space where the x, y, and z values vary from -1.0 to 1.0.

Any coordiantes that fall outside this range will be discarded/clipped and won't be visible on your screen.

NDC coordinates will be transformed to *screen-space coordinates* via the viewport transform.

The resulting screen-space coordinates are then transformed to fragments as inputs to your fragment shader.