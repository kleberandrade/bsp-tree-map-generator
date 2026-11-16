#!/usr/bin/env python3
"""Render README SVGs with Python's standard library and the compiled CLI.
Run from any directory after `make`: python3 tools/render_readme.py.
The partition and tree figures are teaching schematics; the gallery is real output.
"""
from html import escape
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[1]
OUTPUT = ROOT / 'docs' / 'images'
OUTPUT.mkdir(parents=True, exist_ok=True)


def text(x, y, label, size=16, color='#172b4d', anchor='start'):
    return (f'<text x="{x}" y="{y}" font-size="{size}" fill="{color}" '
            f'text-anchor="{anchor}">{escape(label)}</text>')


def rect(x, y, width, height, fill, stroke='none'):
    return (f'<rect x="{x}" y="{y}" width="{width}" height="{height}" '
            f'fill="{fill}" stroke="{stroke}"/>')


def line(x1, y1, x2, y2, color='#52657d', width=2, dashed=False):
    dash = ' stroke-dasharray="6 5"' if dashed else ''
    return (f'<line x1="{x1}" y1="{y1}" x2="{x2}" y2="{y2}" '
            f'stroke="{color}" stroke-width="{width}"{dash}/>')


def save(name, width, height, title, description, content):
    svg = (f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" '
           f'viewBox="0 0 {width} {height}" role="img" aria-labelledby="title desc">'
           f'<title id="title">{escape(title)}</title><desc id="desc">{escape(description)}</desc>'
           f'<rect width="{width}" height="{height}" fill="#ffffff"/>'
           '<g font-family="Arial, Helvetica, sans-serif">' + ''.join(content) + '</g></svg>\n')
    (OUTPUT / name).write_text(svg)


# One conceptual root, split vertically, then split each child horizontally.
parts = [text(24, 32, 'From regions to connected rooms', 23),
         text(24, 58, 'Teaching schematic — not a trace of a particular seed', 14)]
colors = ['#e0ebff', '#d5f3e8', '#fff0c9', '#f3dff5']
for stage, label in enumerate(['1. Partition', '2. Place rooms', '3. Connect siblings']):
    ox, oy = 24 + stage * 312, 112
    parts += [text(ox, 94, label, 18), rect(ox, oy, 280, 200, '#edf1f6', '#52657d')]
    for i, (x, y, w, h) in enumerate([(0, 0, 140, 90), (0, 90, 140, 110),
                                      (140, 0, 140, 120), (140, 120, 140, 80)]):
        parts.append(rect(ox+x, oy+y, w, h, colors[i]))
    if stage:
        for x, y, w, h in [(18,18,85,50),(30,120,90,56),(170,25,84,65),(175,145,80,40)]:
            parts.append(rect(ox+x, oy+y, w, h, '#147d69'))
    if stage == 2:
        # A-B and C-D first, then A-C, as representative points returned upwards.
        for x1,y1,x2,y2 in [(60,43,75,43),(75,43,75,148),
                             (212,57,215,57),(215,57,215,165),
                             (60,43,212,43),(212,43,212,57)]:
            parts.append(line(ox+x1,oy+y1,ox+x2,oy+y2,'#172b4d',5))
    for x1,y1,x2,y2 in [(140,0,140,200),(0,90,140,90),(140,120,280,120)]:
        parts.append(line(ox+x1,oy+y1,ox+x2,oy+y2,dashed=True))
    for label_, x, y in [('A',5,16),('B',5,108),('C',145,16),('D',145,138)]:
        parts.append(text(ox+x,oy+y,label_,13))
parts += [text(24,345,'Dashed lines: partition boundaries. Green: rooms. Dark paths: inclusive L-shaped corridors.',14),
          text(24,370,'Join A–B and C–D, then join A–C to connect both subtrees. Boundaries are not necessarily walls.',14)]
save('partition-steps.svg',960,395,'BSP partition, room and corridor steps',
     'Three schematic panels show the same four regions A B C D, then rooms, then connecting paths.',parts)

nodes = [('Whole interior',480,70),('Left region',250,170),('Right region',710,170),
         ('Room A',135,270),('Room B',365,270),('Room C',595,270),('Room D',825,270)]
parts = [text(24,32,'The spatial splits form a binary tree',23)]
for parent, child in [(0,1),(0,2),(1,3),(1,4),(2,5),(2,6)]:
    _,x,y=nodes[parent]; _,cx,cy=nodes[child]
    parts.append(line(x,y+34,cx,cy))
for i,(label,x,y) in enumerate(nodes):
    parts += [rect(x-95,y,190,40,colors[i-3] if i>=3 else '#edf1f6','#52657d'),
              text(x,y+26,label,16,anchor='middle')]
parts += [text(24,345,'Internal nodes: split regions. Leaves: carve rooms. Return one floor point from each subtree.',15),
          text(24,372,'This implementation keeps the tree in recursive calls; it does not allocate persistent tree nodes.',15)]
save('bsp-tree.svg',960,395,'The binary tree behind the four-region example',
     'One root has left and right children; those have room leaves A B and C D respectively.',parts)

parts=[text(24,32,'Actual generator output',23),
       text(24,58,'Same 40 × 20 grid and seed 42; change depth and minimum room size.',14)]
for index,(depth,room) in enumerate([(0,3),(6,2),(6,6)]):
    rows = subprocess.check_output([str(ROOT/'build/bsp-map'),'40','20',str(depth),str(room),'42'],text=True).splitlines()
    assert len(rows)==20 and all(len(row)==40 and set(row)<=set('#.') for row in rows)
    ox,oy=24+index*312,112
    parts.append(text(ox,94,f'Depth {depth} · min room {room}',18))
    parts.append(rect(ox,oy,280,140,'#172b4d'))
    for y,row in enumerate(rows):
        for x,cell in enumerate(row):
            if cell=='.': parts.append(rect(ox+x*7,oy+y*7,7,7,'#88e0c4'))
parts += [rect(24,280,14,14,'#172b4d'),text(46,292,'Wall (#)',14),
          rect(160,280,14,14,'#88e0c4'),text(182,292,'Floor (.)',14),
          text(24,324,'Depth is a limit, not a target. Larger minimum rooms can stop subdivision earlier.',15),
          text(24,350,'Rooms can touch or merge; the generator guarantees connected floor, not separating walls.',15)]
save('dungeon-examples.svg',960,375,'Three deterministic BSP dungeon examples',
     'Real CLI output for width 40 height 20 seed 42, comparing depth zero, depth six and larger rooms.',parts)
print('Rendered three README SVGs in docs/images/')
