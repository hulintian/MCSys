<template>
  <div class="page">
    <h2>Camera</h2>

    <img :src="videoUrl" />

    <div class="controls">
      <button @click="getDescription" :disabled="loading">
        {{ loading ? "识别中..." : "获取画面描述" }}
      </button>
    </div>

    <div v-if="desc" class="desc-box">
      <h3>前方环境描述</h3>
      <p>{{ desc }}</p>
    </div>
  </div>
</template>

<script setup>
import { ref } from "vue"

const videoUrl = `${location.protocol}//${location.hostname}:5000/video`
const apiBase = `${location.protocol}//${location.hostname}:5000`

const desc = ref("")
const loading = ref(false)

async function getDescription() {
  loading.value = true
  desc.value = ""

  try {
    const res = await fetch(`${apiBase}/snapshot_desc`)
    const data = await res.json()
    desc.value = data.description || "未获取到描述"
  } catch (e) {
    desc.value = "请求失败"
  } finally {
    loading.value = false
  }
}
</script>

<style scoped>
.controls {
  margin-top: 12px;
}

button {
  padding: 6px 12px;
}

.desc-box {
  margin-top: 16px;
  background: #1e1e1e;
  color: #eee;
  padding: 12px;
  border-radius: 6px;
}
</style>
