/** @type {import('tailwindcss').Config} */
export default {
  content: [
    "./src/**/*.{html,js,jsx,ts,tsx}",
    "./public/index.html",
    "./src-tauri/**/*.rs",
  ],
  theme: {
    extend: {
      colors: {
        main: "#560bad",
      },
    },
  },
  plugins: [],
};
