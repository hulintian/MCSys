<template>
  <div class="dashboard">
    <h1>System Health</h1>

    <section class="grid">
      <StatCard label="CPU" :value="health.cpu" />
      <StatCard label="Memory" :value="health.memory" />
      <StatCard label="Disk" :value="health.disk" />
    </section>

    <p class="time">
      Updated at: {{ new Date(health.timestamp * 1000).toLocaleString() }}
    </p>
  </div>
</template>

<script setup>
import { ref, onMounted, onUnmounted } from 'vue'
import {useHealth} from '../components/useHealth.js'
import StatCard from '../components/StatCard.vue'

const {health, loading, error} = useHealth(2000)
</script>

<style>
body {
  margin: 0;
  background: #0b1020;
  color: #fff;
  font-family: system-ui, sans-serif;
}
.container {
  padding: 24px;
}
.grid {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  gap: 16px;
}
.meta {
  margin-top: 16px;
  opacity: 0.6;
}
</style>
