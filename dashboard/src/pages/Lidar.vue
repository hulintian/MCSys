<template>
  <div class="lidar">
    <h1>LiDAR Viewer</h1>

    <div class="status">
      <span :class="connected ? 'ok' : 'bad'">
        {{ connected ? 'connected' : 'disconnected' }}
      </span>
      <span>FPS: {{ fps }}</span>
      <span>Points: {{ points.length }}</span>
    </div>

    <canvas ref="canvas" width="700" height="500"></canvas>
  </div>
</template>

<script setup>
import { ref, watch } from 'vue'
import { useLidar } from '../components/useLidar'

const canvas = ref(null)
const { connected, points, fps } = useLidar()


function draw(pts) {
  const cv = canvas.value
  if (!cv) return
  const ctx = cv.getContext('2d')

  // =========================
  // 基本尺寸
  // =========================
  const w = cv.width
  const h = cv.height
  const cx = w / 2
  const cy = h / 2

  ctx.clearRect(0, 0, w, h)

  // =========================
  // 物理 / 显示参数
  // =========================
  const MAX_RANGE_M = 30          // 最大量程（米）
  const R = Math.min(w, h) * 0.45 // 雷达显示半径（像素）

  // =========================
  // 背景（可选，深色渐变）
  // =========================
  const bg = ctx.createRadialGradient(cx, cy, 0, cx, cy, R)
  bg.addColorStop(0, '#0b1020')
  bg.addColorStop(1, '#050814')
  ctx.fillStyle = bg
  ctx.fillRect(0, 0, w, h)

  // =========================
  // 1️⃣ 距离刻度（同心圆 + 数字）
  // =========================
  const rangeMarks = []

  ctx.strokeStyle = '#2a335a'
  ctx.fillStyle = '#8fa4ff'
  ctx.font = '14px system-ui'
  ctx.textAlign = 'left'
  ctx.textBaseline = 'middle'

  ctx.setLineDash([6, 6])
  for (const m of rangeMarks) {
    if (m > MAX_RANGE_M) continue

    const rr = (m / MAX_RANGE_M) * R

    ctx.beginPath()
    ctx.arc(cx, cy, rr, 0, Math.PI * 2)
    ctx.stroke()

    // 标注在正上方
    ctx.fillText(`${m} m`, cx + 6, cy - rr)
  }
  ctx.setLineDash([])

  // =========================
  // 2️⃣ 方向刻度（十字轴）
  // =========================
  ctx.strokeStyle = '#1f264a'
  ctx.lineWidth = 1

  ctx.beginPath()
  ctx.moveTo(cx - R, cy)
  ctx.lineTo(cx + R, cy)
  ctx.moveTo(cx, cy - R)
  ctx.lineTo(cx, cy + R)
  ctx.stroke()

  ctx.fillStyle = '#aab4ff'
  ctx.font = '15px system-ui'
  ctx.textAlign = 'center'
  ctx.textBaseline = 'middle'

  ctx.fillText('Front', cx, cy - R - 18)
  ctx.fillText('Back',  cx, cy + R + 18)
  ctx.fillText('Left',  cx - R - 36, cy)
  ctx.fillText('Right', cx + R + 36, cy)

  // =========================
  // 3️⃣ 角度刻度短线（每 30°）
  // =========================
  ctx.strokeStyle = '#2a335a'

  for (let deg = 0; deg < 360; deg += 30) {
    const rad = deg * Math.PI / 180 + Math.PI / 2  // 顺时针 90°
    const r1 = R * 0.96
    const r2 = R

    const x1 = cx + r1 * Math.cos(rad)
    const y1 = cy + r1 * Math.sin(rad)
    const x2 = cx + r2 * Math.cos(rad)
    const y2 = cy + r2 * Math.sin(rad)

    ctx.beginPath()
    ctx.moveTo(x1, y1)
    ctx.lineTo(x2, y2)
    ctx.stroke()
  }

  // =========================
  // 4️⃣ 点云（最后画，最清晰）
  // =========================
  ctx.fillStyle = '#00ff88'

  for (const p of pts) {
    // ---- 角度（顺时针 90°）
    const rad =
      -(p.angle / 100) * Math.PI / 180 + Math.PI / 2

    // ---- 距离（原始 /100 → 米）
    const dist_m = p.dist / 100.0
    if (dist_m > MAX_RANGE_M) continue

    // ---- 米 → 像素
    const r_px = (dist_m / MAX_RANGE_M) * R

    const x = cx + r_px * Math.cos(rad)
    const y = cy + r_px * Math.sin(rad)

    ctx.fillRect(x - 1, y - 1, 2, 2)
  }
}

watch(points, (pts) => {
  draw(pts);
})
</script>
