// TourPlanner.java

import org.graphstream.graph.Graph;
import org.graphstream.graph.Node;
import org.graphstream.graph.Edge;
import org.graphstream.graph.implementations.SingleGraph;

import org.graphstream.ui.view.View;
import org.graphstream.ui.view.Viewer;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;

public class TourPlanner extends JFrame {
    private final Graph graph;
    private final Component viewPanel;  // Panel para la vista del grafo
    private final JComboBox<String> cbStart;
    private final JTextArea taResult;

    public TourPlanner() {
        super("Tour Planner");
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setSize(900, 600);

        // 1) Creamos el grafo y le damos estilo
        graph = new SingleGraph("Tour");
        graph.setAttribute("ui.stylesheet",
            "node { fill-color: #1f77b4; size: 24px; text-alignment: under; text-size: 14; }" +
            "edge { text-alignment: above; text-size: 12; }"
        );

        // 2) Lo enganchamos al panel Swing con Viewer (GraphStream 1.3)
        Viewer viewer = new Viewer(graph, Viewer.ThreadingModel.GRAPH_IN_GUI_THREAD);
        //viewer.enableAutoLayout();  // DESACTIVADO para evitar error con ParticleBoxListener

        View view = viewer.addDefaultView(false);
        viewPanel = (Component) view;
        getContentPane().add(viewPanel, BorderLayout.CENTER);

        // 3) Panel de controles
        JPanel controls = new JPanel(new FlowLayout(FlowLayout.LEFT, 8, 8));
        cbStart  = new JComboBox<>();
        JButton bAddNode = new JButton("Añadir Nodo");
        JButton bDelNode = new JButton("Eliminar Nodo");
        JButton bAddEdge = new JButton("Añadir Ruta");
        JButton bDelEdge = new JButton("Eliminar Ruta");
        JButton bBFS     = new JButton("Recorrer BFS");
        JButton bDFS     = new JButton("Recorrer DFS");
        taResult = new JTextArea(5, 50);
        taResult.setEditable(false);
        taResult.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 12));

        controls.add(new JLabel("Inicio:"));
        controls.add(cbStart);
        controls.add(bAddNode);
        controls.add(bDelNode);
        controls.add(bAddEdge);
        controls.add(bDelEdge);
        controls.add(bBFS);
        controls.add(bDFS);

        getContentPane().add(controls, BorderLayout.NORTH);
        getContentPane().add(new JScrollPane(taResult), BorderLayout.SOUTH);

        // 4) Handlers

        bAddNode.addActionListener(e -> {
            String name = JOptionPane.showInputDialog(this, "Nombre del lugar turístico:");
            if (name != null && !name.isBlank() && graph.getNode(name) == null) {
                Node n = graph.addNode(name);
                n.setAttribute("ui.label", name);

                // Asignar posición aleatoria para que nodos no se amontonen
                double x = Math.random() * 800;
                double y = Math.random() * 500;
                n.setAttribute("x", x);
                n.setAttribute("y", y);

                cbStart.addItem(name);
            }
        });

        bDelNode.addActionListener(e -> {
            String name = (String) cbStart.getSelectedItem();
            if (name != null && graph.getNode(name) != null) {
                graph.removeNode(name);
                cbStart.removeItem(name);
            }
        });

        bAddEdge.addActionListener(e -> {
            String a = JOptionPane.showInputDialog(this, "Nodo origen:");
            String b = JOptionPane.showInputDialog(this, "Nodo destino:");
            String t = JOptionPane.showInputDialog(this, "Tiempo (min):");
            if (a!=null && b!=null && t!=null
             && graph.getNode(a)!=null && graph.getNode(b)!=null) {
                double time = Double.parseDouble(t);
                String id1 = a + "_" + b;
                String id2 = b + "_" + a;
                Edge e1 = graph.addEdge(id1, a, b);
                Edge e2 = graph.addEdge(id2, b, a);
                e1.setAttribute("time", time);
                e2.setAttribute("time", time);
                e1.setAttribute("ui.label", t);
                e2.setAttribute("ui.label", t);
            }
        });

        bDelEdge.addActionListener(e -> {
            String id = JOptionPane.showInputDialog(this, "ID de la ruta (origen_destino):");
            if (id!=null && graph.getEdge(id)!=null) {
                graph.removeEdge(id);
                String inv = id.contains("_") ? id.split("_",2)[1] + "_" + id.split("_",2)[0] : null;
                if (inv!=null && graph.getEdge(inv)!=null)
                    graph.removeEdge(inv);
            }
        });

        bBFS.addActionListener(e -> {
            String start = (String) cbStart.getSelectedItem();
            if (start != null) {
                taResult.setText(recorridoBFS(graph.getNode(start)));
            }
        });

        bDFS.addActionListener(e -> {
            String start = (String) cbStart.getSelectedItem();
            if (start != null) {
                taResult.setText(recorridoDFS(graph.getNode(start)));
            }
        });
    }

    // Recorrido en anchura
    private String recorridoBFS(Node inicio) {
        List<Node> cola   = new ArrayList<>();
        List<Node> vistos = new ArrayList<>();
        cola.add(inicio);
        vistos.add(inicio);

        StringBuilder sb = new StringBuilder("--- BFS ---\n");
        double totalTime = 0;

        while (!cola.isEmpty()) {
            Node u = cola.remove(0);
            sb.append(u.getId()).append("\n");
            for (Edge e : u.getEachEdge()) {
                Node v = e.getOpposite(u);
                if (!vistos.contains(v)) {
                    vistos.add(v);
                    cola.add(v);
                    totalTime += e.getNumber("time");
                }
            }
        }
        sb.append(String.format("Tiempo total: %.0f min", totalTime));
        return sb.toString();
    }

    // Recorrido en profundidad
    private String recorridoDFS(Node inicio) {
        List<Node> stack  = new ArrayList<>();
        List<Node> vistos = new ArrayList<>();
        stack.add(inicio);

        StringBuilder sb = new StringBuilder("--- DFS ---\n");
        double totalTime = 0;

        while (!stack.isEmpty()) {
            Node u = stack.remove(stack.size()-1);
            if (!vistos.contains(u)) {
                vistos.add(u);
                sb.append(u.getId()).append("\n");
                for (Edge e : u.getEachEdge()) {
                    Node v = e.getOpposite(u);
                    if (!vistos.contains(v)) {
                        stack.add(v);
                        totalTime += e.getNumber("time");
                    }
                }
            }
        }
        sb.append(String.format("Tiempo total: %.0f min", totalTime));
        return sb.toString();
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            new TourPlanner().setVisible(true);
        });
    }
}
