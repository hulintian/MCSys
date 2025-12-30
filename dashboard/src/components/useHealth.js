import { ref, onMounted, onUnmounted } from 'vue'

export function useHealth(interval = 2000) {
  const health = ref({
    cpu: 0,
    memory: 0,
    disk: 0,
    timestamp: 0
  })

  const loading = ref(true)
  const error = ref(null)

  let timer

  async function fetchHealth () {
    try {
        console.log('fetch /api/health')
      const res = await fetch('/api/health')
        console.log('status ', res.status)
      if (!res.ok) throw new Error(res.statusText)
      health.value = await res.json()
      error.value = null
    } catch (e) {
      error.value = e
    } finally {
      loading.value = false
    }
  }

  onMounted(() => {
    fetchHealth()
    timer = setInterval(fetchHealth, interval)
  })

  onUnmounted(() => clearInterval(timer))

  return { health, loading, error }
}

