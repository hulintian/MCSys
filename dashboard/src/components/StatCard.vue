<template>
  <div class="card">
    <div class="label">{{ label }}</div>
    <div class="value">{{ value }}%</div>
  </div>
</template>

<script setup>
defineProps({
  label: String,
  value: Number
})
</script>

<style scoped>
.card {
  padding: 16px;
  border-radius: 12px;
  background: #151b36;
  color: #fff;
}
.label {
  opacity: 0.7;
}
.value {
  font-size: 28px;
  font-weight: bold;
}
</style>

