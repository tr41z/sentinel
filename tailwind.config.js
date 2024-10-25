/** @type {import('tailwindcss').Config} */
export default {
  content: [
    "./src/**/*.{html,js,jsx,ts,tsx}", // Include all JS/TS files in src
    "./public/index.html", // Include the main HTML file
    "./src-tauri/**/*.rs", // Include Tauri Rust files if needed for CSS-in-Rust
  ],
  theme: {
    extend: {},
  },
  plugins: [],
};
