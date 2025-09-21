#!/usr/bin/env nu

def generate_layers_h_names [] {
  get LAY
  | uniq
  | enumerate
  | each {|r| $'#define CI_LAY_($r.item) ($r.index)'}
  | str join "\n"
}

def generate_layers_h_keymaps [] {
  let layers = $in | chunk-by { get LAY }

  $layers
  | enumerate
  | each {|l|
      let keys = $l.item
      | reject LAY
      | transpose -i
      | values
      | flatten
      | compact -e
      | str join ','

      $"  // ($l.item.LAY | first)\n  [($l.index)] = LAYOUT_split_3x6_3_ex2\(($keys)\)"
    }
  | str join ",\n\n"
}

def generate_layers_h [] {
  let layers = $in

  [
    '#pragma once'
    ''
    ($layers | generate_layers_h_names)
    ''
    'const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {'
    ($layers | generate_layers_h_keymaps)
    '};'
  ] | str join "\n"
}

def generate_layers_rgb_h [keyboard_json: string] {
  let layers = $in | chunk-by { get LAY } | each { reject LAY }

  let layout = open $keyboard_json
  | get rgb_matrix
  | get layout
  | get matrix

  let matrices = $layers | each {
    transpose -i
    | chunks 7
    | each { transpose -i }
    | update 1 { transpose -i | reverse | transpose -i }
    | flatten
  }

  let flat_colors = $matrices
  | each {|m|
      $layout
      | each {|l|
          $m | get $l.0 | get $'column($l.1)'
        }
    }

  let color_values = $flat_colors
  | enumerate
  | each {|l|
      $'  [($l.index)] = {($l.item | each {|c| $'{($c)}'} | str join ',')}'
    }
  | str join ",\n\n"

  $"#pragma once\n\nconst uint8_t PROGMEM ci_layers_colors[][46][3] = {\n($color_values)\n};"
}

def main [
  infile: string,
  keyboard_json: string,
  --quiet (-q),
  --keymaps-out (-k): string,
  --rgb-out (-r): string
] {
  let layers = open -r $infile | from csv -c '#' -s ';' | reject G1 G2

  let keymaps = $layers
  | where SEC == 'KEYS'
  | reject SEC
  | generate_layers_h

  let rgb = $layers
  | where SEC == 'RGB'
  | reject SEC
  | generate_layers_rgb_h $keyboard_json

  if ($keymaps_out | is-not-empty) {
    $keymaps | save -f $keymaps_out
  }

  if ($rgb_out | is-not-empty) {
    $rgb | save -f $rgb_out
  }

  if not $quiet {
    return { keymaps: $keymaps, rgb: $rgb } | to yaml
  }
}
