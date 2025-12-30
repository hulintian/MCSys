<template>
  <div class="home">
    <header class="header">
      <h1>Dashboard</h1>
      <p class="subtitle">System overview & monitoring</p>
    </header>

    <section class="cards">
      <!-- System Health -->
      <div class="card active" @click="$router.push('/health')">
        <h3>System Health</h3>
        <p class="desc">CPU / Memory / Disk</p>
        <p class="hint">Click to view details →</p>
      </div>

      <!-- LiDAR -->
      <div class="card active" @click="$router.push('/lidar')">
        <h3>LiDAR Viewer</h3>
        <p class="desc">Point Cloud / Radar</p>
        <p class="hint">Click to view details →</p>
      </div>

      <!-- Services -->
      <div class="card active" @click="$router.push('/camera')">
        <h3>Camera</h3>
        <p class="desc"> Camera / View</p>
        <p class="hint">Click to view details →</p>
      </div>
    </section>
  </div>
</template>

<style scoped>
/* ===== Header ===== */
.header {
  margin-bottom: 36px;
}

.header h1 {
  margin: 0;
  font-size: clamp(32px, 4vw, 48px);
  letter-spacing: -0.5px;
}

.subtitle {
  margin-top: 8px;
  font-size: 14px;
  opacity: 0.6;
}

/* ===== 卡片布局（横向自适应） ===== */
.cards {
  display: grid;
  grid-template-columns: repeat(
    auto-fit,
    minmax(300px, 1fr)
  );
  gap: 32px;
}

/* ===== 卡片基础态 ===== */
.card {
  position: relative;

  background: linear-gradient(
    180deg,
    rgba(30, 38, 82, 0.85) 0%,
    rgba(18, 24, 58, 0.85) 100%
  );

  border-radius: 18px;
  padding: 28px;

  box-shadow:
    0 12px 32px rgba(0, 0, 0, 0.45);

  transition:
    transform 0.2s ease,
    box-shadow 0.2s ease,
    background 0.2s ease;
}

.card h3 {
  margin: 0;
  font-size: 18px;
}

.desc {
  margin-top: 10px;
  font-size: 14px;
  opacity: 0.75;
  transition: opacity 0.2s ease;
}

.hint {
  margin-top: 18px;
  font-size: 13px;
  opacity: 0.6;
  transition: opacity 0.2s ease;
}

/* ===== Hover / Active ===== */
.card.active {
  cursor: pointer;
}

.card.active:hover {
  transform: translateY(-8px);

  background: linear-gradient(
    180deg,
    rgba(45, 58, 140, 0.9) 0%,
    rgba(26, 34, 92, 0.9) 100%
  );

  box-shadow:
    0 20px 60px rgba(0, 0, 0, 0.65),
    0 0 0 1px rgba(120, 140, 255, 0.08);
}

.card.active:hover .desc,
.card.active:hover .hint {
  opacity: 0.95;
}

/* ===== 内描边（静态层次） ===== */
.card::after {
  content: '';
  position: absolute;
  inset: 0;
  border-radius: 18px;
  pointer-events: none;
  box-shadow:
    inset 0 0 0 1px rgba(255, 255, 255, 0.03);
}

/* ===== 禁用态 ===== */
.card.disabled {
  opacity: 0.45;
  filter: grayscale(0.2);

  cursor: not-allowed;
}
</style>

