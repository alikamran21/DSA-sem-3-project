<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width,initial-scale=1" />
  <title>DSA-sem-3-project — BFIDS Scope Document</title>
  <style>
    :root{
      --bg:#ffffff;
      --text:#0e1116;
      --muted:#4b5563;
      --accent:#0b63d6;
      --panel:#f6f8fb;
      --table-head:#eef3fb;
      --border:#e6e9ee;
      --mono: "Courier New", Courier, monospace;
      --maxw:900px;
    }
    body{
      background:var(--bg);
      color:var(--text);
      font-family:Inter, "Segoe UI", Roboto, Arial, sans-serif;
      line-height:1.55;
      margin:32px;
      display:flex;
      justify-content:center;
    }
    .container{
      width:100%;
      max-width:var(--maxw);
      box-shadow:0 6px 28px rgba(12,18,27,0.06);
      border-radius:8px;
      background:#fff;
      padding:28px;
      box-sizing:border-box;
    }
    header h1{
      margin:0 0 6px 0;
      font-size:20px;
      letter-spacing:0.2px;
    }
    header p{
      margin:0;
      color:var(--muted);
      font-size:13px;
    }
    section{
      margin-top:20px;
    }
    h2{
      font-size:16px;
      margin:18px 0 8px 0;
      color:var(--text);
      border-left:4px solid var(--accent);
      padding-left:10px;
      font-weight:600;
    }
    p.lead{
      margin:0 0 8px 0;
      color:var(--muted);
      font-size:14px;
    }
    table{
      width:100%;
      border-collapse:collapse;
      margin-top:12px;
      font-size:14px;
    }
    th, td{
      padding:10px 12px;
      border:1px solid var(--border);
      text-align:left;
      vertical-align:top;
    }
    thead th{
      background:var(--table-head);
      font-weight:600;
    }
    .file-table td{
      font-family:var(--mono);
      white-space:nowrap;
    }
    .two-col {
      display:grid;
      grid-template-columns:1fr 332px;
      gap:20px;
      align-items:start;
    }
    .badge{
      display:inline-block;
      padding:6px 10px;
      background:var(--panel);
      border-radius:6px;
      color:var(--muted);
      font-size:12px;
    }
    ul {
      margin:8px 0 0 20px;
    }
    pre{
      background:#0f1724;
      color:#e6eef8;
      padding:12px;
      border-radius:6px;
      overflow:auto;
      font-family:var(--mono);
      font-size:13px;
    }
    .center {
      text-align:center;
    }
    footer{
      margin-top:18px;
      color:var(--muted);
      font-size:13px;
    }
    @media (max-width:900px){
      .two-col{ grid-template-columns:1fr; }
    }
  </style>
</head>
<body>
  <div class="container" role="main">
    <header>
      <h1>DSA-sem-3-project</h1>
      <p>Title: Behavioral Fingerprint-Based Intrusion Detection System (BFIDS)</p>
      <p>Team: Fatima Rahman, Ali Kamran</p>
    </header>

    <section>
      <h2>Problem Statement</h2>
      <p class="lead">
        Modern cybersecurity systems often rely on static signatures or heuristic methods that fail to identify new or insider threats.
        Each authenticated user exhibits characteristic behavior — command sequences, typing intervals, and action transitions — which can be modeled
        as a behavioral fingerprint. This project implements a Behavioral Fingerprint-Based Anomaly Detection System using only C++ and core
        Data Structures and Algorithms (DSA). The system will learn normal user behavior, generate behavioral fingerprints, and detect anomalies by
        comparing new sessions against stored profiles. The implementation emphasizes DSA primitives and algorithms; no external machine learning
        frameworks or third-party libraries will be used.
      </p>
    </section>

    <section>
      <h2>List of Data Structures and Algorithms</h2>

      <table>
        <thead>
          <tr>
            <th>Data Structure / Algorithm</th>
            <th>Application in Project</th>
          </tr>
        </thead>
        <tbody>
          <tr><td>Array / Dynamic Array</td><td>Store time-series samples, latency buffers, raw behavioral data</td></tr>
          <tr><td>Singly &amp; Doubly Linked Lists</td><td>Maintain session logs and reversible action histories</td></tr>
          <tr><td>Stack (LIFO)</td><td>Track nested actions (open/close sequences, call stacks)</td></tr>
          <tr><td>Queue (FIFO)</td><td>Ordered event logs for temporal sequence validation</td></tr>
          <tr><td>Circular Queue</td><td>Sliding windows and bounded recent-session buffers</td></tr>
          <tr><td>Priority Queue / Heap (Min/Max)</td><td>Rank anomaly scores and generate priority alerts</td></tr>
          <tr><td>Hash Table (Open Addressing)</td><td>Map user IDs to fingerprints for constant-time retrieval</td></tr>
          <tr><td>Binary Search Tree (BST)</td><td>Store sorted frequency metrics and support range queries</td></tr>
          <tr><td>AVL Tree</td><td>Balanced profile storage for guaranteed logarithmic lookup</td></tr>
          <tr><td>Trie (Prefix Tree)</td><td>Store sequences of commands or key transitions for prefix matching</td></tr>
          <tr><td>Graph (Adjacency List / Matrix)</td><td>Model action transitions; support DFS/BFS and path analysis</td></tr>
          <tr><td>Disjoint Set (Union–Find)</td><td>Cluster related behaviors or correlated processes</td></tr>
          <tr><td>Sorting Algorithms</td><td>Bubble, Selection, Insertion, Merge, Quick, Heap — used for ranking and reporting</td></tr>
          <tr><td>Searching Algorithms</td><td>Linear, Binary, Interpolation — used for command and profile lookups</td></tr>
          <tr><td>Divide &amp; Conquer</td><td>Implemented via Merge Sort and Quick Sort</td></tr>
          <tr><td>Greedy Algorithms</td><td>Threshold selection, top-K alert selection</td></tr>
          <tr><td>Dynamic Programming</td><td>Edit distance and sequence similarity for fingerprint comparison</td></tr>
          <tr><td>Graph Traversal</td><td>DFS and BFS for exploring transition graphs</td></tr>
          <tr><td>Shortest Path (Dijkstra)</td><td>Compute deviation cost between behavior paths</td></tr>
          <tr><td>Backtracking</td><td>Reconstruct plausible action sequences given partial observations</td></tr>
          <tr><td>Recursion</td><td>Used in traversals, sorting and divide-and-conquer routines</td></tr>
          <tr><td>File I/O Operations</td><td>Persist fingerprints, logs and anomaly reports</td></tr>
        </tbody>
      </table>
    </section>

    <section>
      <h2>Expected Outcomes</h2>

      <table>
        <thead>
          <tr>
            <th>Outcome</th>
            <th>Description</th>
          </tr>
        </thead>
        <tbody>
          <tr><td>Modular C++ Implementation</td><td>Complete system demonstrating behavioral anomaly detection using classic DSA</td></tr>
          <tr><td>Behavioral Fingerprints</td><td>Generate and compare fingerprints for multiple simulated users</td></tr>
          <tr><td>Real-Time Detection</td><td>Identify abnormal sessions and rank them by severity using heaps</td></tr>
          <tr><td>Algorithmic Analysis</td><td>Provide time and space complexity analysis for core modules</td></tr>
          <tr><td>Performance Reporting</td><td>Document detection accuracy and runtime performance</td></tr>
          <tr><td>Scalability &amp; Extensibility</td><td>Design prepared for integration with larger cybersecurity systems</td></tr>
        </tbody>
      </table>
    </section>

    <section>
      <h2>Data Repository</h2>
      <p class="lead">All behavioral data will be stored as plain text logs and fingerprint records in the <code>/data</code> directory.</p>

      <table>
        <thead>
          <tr>
            <th>Field</th>
            <th>Example</th>
          </tr>
        </thead>
        <tbody>
          <tr><td>Timestamp</td><td>2025-10-06 22:45:31</td></tr>
          <tr><td>UserID</td><td>U101</td></tr>
          <tr><td>Action</td><td>Open_File</td></tr>
          <tr><td>Process</td><td>explorer.exe</td></tr>
          <tr><td>Duration</td><td>120ms</td></tr>
          <tr><td>Next_Action</td><td>Edit_Doc</td></tr>
          <tr><td>Status</td><td>Normal / Deviated</td></tr>
        </tbody>
      </table>

      <p class="lead" style="margin-top:10px">Files stored in repository</p>
      <ul>
        <li><code>data/user_logs.txt</code></li>
        <li><code>data/fingerprints.txt</code></li>
        <li><code>data/anomalies.txt</code></li>
      </ul>
    </section>

    <section>
      <h2>Proposed File Structure</h2>

      <table class="file-table">
        <thead>
          <tr><th>File / Folder</th></tr>
        </thead>
        <tbody>
          <tr><td><code>main.cpp</code></td></tr>
          <tr><td><code>include/user_action.h</code></td></tr>
          <tr><td><code>include/stack_monitor.h</code></td></tr>
          <tr><td><code>include/queue_logger.h</code></td></tr>
          <tr><td><code>include/linked_list.h</code></td></tr>
          <tr><td><code>include/trie_behavior.h</code></td></tr>
          <tr><td><code>include/graph_transition.h</code></td></tr>
          <tr><td><code>include/bst_frequency.h</code></td></tr>
          <tr><td><code>include/avl_profile.h</code></td></tr>
          <tr><td><code>include/hash_profiles.h</code></td></tr>
          <tr><td><code>include/heap_alerts.h</code></td></tr>
          <tr><td><code>include/disjoint_cluster.h</code></td></tr>
          <tr><td><code>include/sorting_algorithms.h</code></td></tr>
          <tr><td><code>include/searching_algorithms.h</code></td></tr>
          <tr><td><code>include/dynamic_edit.h</code></td></tr>
          <tr><td><code>include/traversal_algorithms.h</code></td></tr>
          <tr><td><code>include/fingerprint_manager.h</code></td></tr>
          <tr><td><code>include/anomaly_detection.h</code></td></tr>
          <tr><td><code>include/utils.h</code></td></tr>
          <tr><td><code>include/file_io.h</code></td></tr>
          <tr><td><code>src/</code></td></tr>
          <tr><td><code>data/user_logs.txt</code></td></tr>
          <tr><td><code>data/fingerprints.txt</code></td></tr>
          <tr><td><code>data/anomalies.txt</code></td></tr>
          <tr><td><code>tests/test_stack.cpp</code></td></tr>
          <tr><td><code>tests/test_queue.cpp</code></td></tr>
          <tr><td><code>tests/test_trie.cpp</code></td></tr>
          <tr><td><code>tests/test_graph.cpp</code></td></tr>
          <tr><td><code>tests/test_bst.cpp</code></td></tr>
          <tr><td><code>tests/test_manager.cpp</code></td></tr>
          <tr><td><code>README.md</code></td></tr>
        </tbody>
      </table>
    </section>

    <section>
      <h2>Five-Week Plan</h2>

      <table>
        <thead>
          <tr>
            <th>Week</th>
            <th>Focus Area</th>
            <th>Modules / Deliverables</th>
          </tr>
        </thead>
        <tbody>
          <tr><td>Week 1</td><td>Core I/O and session structure</td><td>Implement <code>user_action</code>, <code>stack_monitor</code>, <code>queue_logger</code>, <code>file_io</code></td></tr>
          <tr><td>Week 2</td><td>Behavioral modeling</td><td>Develop <code>linked_list</code>, <code>trie_behavior</code>, <code>graph_transition</code></td></tr>
          <tr><td>Week 3</td><td>Fingerprint management</td><td>Implement <code>hash_profiles</code>, <code>bst_frequency</code>, <code>avl_profile</code></td></tr>
          <tr><td>Week 4</td><td>Detection &amp; analysis</td><td>Integrate <code>heap_alerts</code>, <code>dynamic_edit</code>, sorting/searching modules</td></tr>
          <tr><td>Week 5</td><td>Testing &amp; reporting</td><td>Complexity evaluation, optimization, final report</td></tr>
        </tbody>
      </table>
    </section>

    <section>
      <h2>Conclusion</h2>
      <p class="lead">
        The proposed project integrates a comprehensive set of Data Structures and Algorithms in the design of a Behavioral Fingerprint-Based Anomaly Detection System.
        The scope is focused and feasible for a five-week implementation using standard C++. The deliverables include a modular codebase, unit tests, complexity analysis and a final report.
      </p>
    </section>

    <footer class="center">
      <small>Prepared by: Fatima Rahman &amp; Ali Kamran — CS221 Semester Project</small>
    </footer>
  </div>
</body>
</html>
