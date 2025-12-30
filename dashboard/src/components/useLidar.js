import { ref, onMounted, onUnmounted } from 'vue'

export function useLidar() {
  const connected = ref(false)
  const points = ref([])      // [{angle, dist}]
  const fps = ref(0)

  let ws = null
  let frameCount = 0
  let fpsTimer = null

  function connect() {

    const wsProto = location.protocol === "https:" ? "wss" : "ws";
    const wsUrl = `${wsProto}://${location.hostname}:9002`;
    const ws = new WebSocket(wsUrl);
    // const url = `ws://127.0.0.1:9002`
    // ws = new WebSocket(url)
    ws.binaryType = 'arraybuffer'

    ws.onopen = () => {
      connected.value = true
    }

    ws.onclose = () => {
      connected.value = false
    }

    ws.onerror = () => {
      connected.value = false
    }

    ws.onmessage = (ev) => {
      decodeBinary(ev.data)
      frameCount++
    }
  }

  function decodeBinary(buffer) {
    const view = new DataView(buffer)
    let off = 0

    // uint32 timestamp（你可以不用）
    off += 4

    // uint16 点数
    const n = view.getUint16(off, true)
    off += 2

    const arr = new Array(n)
    for (let i = 0; i < n; i++) {
      const angle = view.getUint16(off, true); off += 2
      const dist  = view.getUint16(off, true); off += 2
      arr[i] = { angle, dist }
    }

    points.value = arr
  }

  onMounted(() => {
    connect()

    fpsTimer = setInterval(() => {
      fps.value = frameCount
      frameCount = 0
    }, 1000)
  })

  onUnmounted(() => {
    if (ws) ws.close()
    if (fpsTimer) clearInterval(fpsTimer)
  })

  return {
    connected,
    points,
    fps
  }
}

// useLidar()
