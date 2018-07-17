const vertical = '\u2502'
const branch = '\u251C'
const arrayEnd = '\u2510'
const endBranch = '\u2514'
const horizontal = '\u2500'

const doubleVertical = '\u2551'
const doubleBranch = '\u255F'
const doubleArrayEnd = '\u2556'
const doubleEndBranch = '\u2559'

const nextBranch = (value, left, objectSelector, otherSelector) => {
  if(Array.isArray(value)){
    return treeArray(value, left)
  }else if(typeof value == 'object'){
    return treeObject(value, left, objectSelector)
  }else{
    return treeOther(value, left, otherSelector)
  }
}

const treeOther = (other, left, selector) => {
  return left + selector + other + '\n' + left + '\n'
}

const treeObject = (obj, left, selector) => {
  let buffer = ''
  Object.keys(obj).forEach((key, index) => {
    const lastItem = index == Object.keys(obj).length - 1
    const nextLeft = left + (lastItem ? ' ': vertical) + ' '
    const nextSelector = (lastItem ? endBranch : branch) + horizontal
    const nextSelectorOther = endBranch + horizontal

    buffer += left + (lastItem ? endBranch : branch) + horizontal + key + '\n'

    buffer += nextBranch(obj[key], nextLeft, nextSelector, nextSelectorOther)
  })
  return buffer
}

const treeArray = (array, left) => {
  let buffer = ''
  array.forEach((item, index) => {
    const lastItem = index == array.length - 1
    const nextLeft = left + (lastItem ? ' ': doubleVertical) + ' '
    const nextSelector = (lastItem ? endBranch : branch) + horizontal
    const nextSelectorOther = endBranch + horizontal

    buffer += left + (lastItem ? doubleEndBranch: doubleBranch) + horizontal + arrayEnd + '\n'
    buffer += nextBranch(item, nextLeft, nextSelector, nextSelectorOther)
  })
  return buffer
}

const tree = value => {
  return nextBranch(value, '', '', '')
}

module.exports = tree
