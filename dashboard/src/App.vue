<template>
  <div class="app">
    <aside class="sidebar">
      <h2>Dashboard</h2>

      <router-link to="/">Home</router-link>
      <router-link to="/health">Health</router-link>
      <router-link to="/lidar">LiDAR</router-link> 
      <router-link to="/camera">Camera</router-link> 
    </aside>

    <main class="content">
        <div class="content-inner">
            <router-view />
        </div>
    </main>
  </div>
</template>

<style>
/* 全局 */
body {
  margin: 0;
  background: radial-gradient(circle at 30% 20%, #121a3a, #060914);
  color: #fff;
  font-family: system-ui, sans-serif;
  width: 100%;
  height: 100%;
  margin: 0;
}

/* Layout root */
.app {
  display: flex;
}

/* Sidebar */
.sidebar {
  position: fixed;          /* 🔥 关键 */
  left: 0;
  top: 0;
  bottom: 0;

  width: 220px;
  padding: 20px 16px;

  background: linear-gradient(
    180deg,
    #141b3c 0%,
    #0c1128 100%
  );

  box-shadow: 4px 0 20px rgba(0, 0, 0, 0.45);
}

/* Sidebar content */
.sidebar h2 {
  margin: 0 0 24px;
  font-size: 18px;
}

.sidebar a {
  display: block;
  margin: 10px 0;
  color: #fff;
  text-decoration: none;
  opacity: 0.7;
  transition: opacity 0.15s ease;
}

.sidebar a.router-link-active {
  opacity: 1;
  font-weight: 600;
}

.sidebar a:hover {
  opacity: 1;
}

/* Main content */
.content {
  margin-left: 220px;       /* 🔥 给 sidebar 留位置 */
  padding: 32px;
  flex: 1;
  min-height: 100vh;

  display: flex;
  justify-content: center;
}

.content-inner {
  width: 100%;
  max-width: 1280px;   /* 🔥 控制大屏宽度 */
}

</style>


